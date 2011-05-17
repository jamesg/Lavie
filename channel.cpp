#include "channel.h"
#include "stringhashtable.h"
#include <vector>

void channel::addPlugin(plugin* p) {
	plugins.push_back(p);
}

int channel::handleCommand(string nick, string channel, vector<string> words)
{

	// send the command to every per-channel plugin in the channel
	if (commandTable.contains(words[0])) {
		if (commandTable.get(words[0])->handleCommand(nick, channel, words) < 1) { // command was not wanted
			commandTable.remove(words[0]);
		} else {
			return 1;
		}
	}
	// command has not yet been handled, look through the whole plugin list
	vector< plugin* >::iterator i;
	for (i = plugins.begin(); i != plugins.end(); i++) {
		if ((*i)->handleCommand(nick, channel, words) > 0) {
			commandTable.insert(words[0], *i);
			return 1;
		}
	}
}

int channel::handleMessage(string nick, string channel, vector<string> words)
{

	return 0;
}

int channel::doTick()
{
	// doTick() on every per-channel plugin in the channel
	int count = 0;
	for(int i=0; i < plugins.size(); ++i)
	{
		count += plugins.at(i)->doTick();
	}
	return count;
}

int channel::startupOptions(vector<string> args)
{
}
