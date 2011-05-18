#define BOT_NICK "Lavie"
#define BOT_DESC "An IRC bot made by zed0 and Zaer"
#define COMMAND_CHAR '-'
#define INITIAL_CHAN "#stuff"

#include <cstdlib>
#include <sstream>
#include <fstream>
#include <ctime>
#include "irc.h"
#include "http.h"
#include "stringUtils.h"
#include "plugin.h"
#include "channel.h"
#include "stringhashtable.h"

using std::ifstream;
using std::ios_base;

int handleAllCommands(string nick, string channel, vector<string> words);
int handleAllMessages(string nick, string channel, vector<string> words);
//Should return 1 if it did anything, oterwise 0.
int handleAllStartupOptions(vector<string> args);

irc ircNet;

channel globalPlugins;
vector< channel* > channels;
stringhashtable< channel* > channelTable;

bool registerChannel(string n, channel* c) {
	channels.push_back(c);
	return channelTable.insert(n, c);
	return false;
}

int main(int argc, char *argv[])
{
	srand(time(NULL));
	vector<string> args(argv, argv + argc);
	if(args.size() <3)
	{
		cerr << "usage: " + string(argv[0]) + " hostname port [options]" << endl;
		return 1;
	}
	ircNet.setNick(BOT_NICK);
	ircNet.setDesc(BOT_DESC);
	ircNet.joinChannel(INITIAL_CHAN);


	// set up global plugins
	globalPlugins.addPlugin(new replyPlugin());
	globalPlugins.addPlugin(new timePlugin());
	globalPlugins.addPlugin(new flipPlugin());
	globalPlugins.addPlugin(new countPlugin());
	globalPlugins.addPlugin(new ircExtraPlugin());
	//alias plugin should go last to stop other commands being realiased.
	globalPlugins.addPlugin(new aliasPlugin());

	handleAllStartupOptions(args);
	ircNet.connect(argv[1], argv[2]);
	string message;
	while(true)
	{
		// do tick
		vector< channel* >::iterator ii;
		for (ii = channels.begin(); ii != channels.end(); ii++) {
			(*ii)->doTick();
		}
		globalPlugins.doTick();


		if(ircNet.checkMessages(message))
		{
			if(message.find("PRIVMSG",0) != string::npos)
			{
				vector<string> words = stringUtils::msgWords(message);
				if(words.at(0).at(0) == COMMAND_CHAR) //if the line begins with our command char then handle commands
				{
					words.at(0).erase(0,1); //strip the command character off the front
					handleAllCommands(stringUtils::msgNick(message), stringUtils::msgChannel(message), words);
				}
				handleAllMessages(stringUtils::msgNick(message), stringUtils::msgChannel(message), words);
			}
		}
	}
	return 0;
}

int handleAllCommands(string nick, string channelName, vector<string> words)
{
	// per-channel plugins
	if (!channelTable.contains(channelName)) {
		channel* c;
		c = new channel();
		channels.push_back(c);
		channelTable.insert(channelName, c);
	}

	if (channelTable.get(channelName)->handleCommand(nick, channelName, words) > 0) return 1;
	
	// global plugins
	return globalPlugins.handleCommand(nick, channelName, words);
}

int handleAllMessages(string nick, string channel, vector<string> words)
{
	// or not (for now).
	return 0;
}

int handleAllStartupOptions(vector<string> args)
{
	globalPlugins.startupOptions(args);
	return 0;
}
