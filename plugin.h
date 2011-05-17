#ifndef PLUGIN_H
#define PLUGIN_H

#include "irc.h"
#include "stringUtils.h"

extern irc ircNet;
extern int handleAllCommands(string nick, string channel, vector<string> words);
extern int handleAllMessages(string nick, string channel, vector<string> words);
extern int handleAllMessages(string nick, string channel, vector<string> words);
class plugin
{
	private:
	public:
		//bool perChannel;
		//handle a command, return 0 for unmatched, 1 for matched and continue, 2 for matched and stop
		virtual int handleCommand(string nick, string channel, vector<string> words)
		{
			return 0;
		}
		//handle a message, return 0 for unmatched, 1 for matched and continue, 2 for matched and stop
		virtual int handleMessage(string nick, string channel, vector<string> words)
		{
			return 0;
		}
		//run every tick to allow plugins to manipulate variables etc.
		virtual int doTick()
		{
			return 0;
		}
		//gets passed the entire list of arguments with which the program was started to allow plugins to use command line options.
		//return 0 for unmatched, 1 for matched
		virtual int startupOptions(vector<string> args)
		{
			cout << "FAIL FAIL FAIL FAIL FAIL" << endl;
			return 0;
		}
		virtual vector< string > commandList()
		{
			return vector< string >();
		}
};

#endif
