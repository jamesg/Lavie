#ifndef PLUGINS_H
#define PLUGINS_H
#include <vector>
#include <string>

#include "plugin.h"

#include "plugins/filePlugin.h"
#include "plugins/replyPlugin.h"
#include "plugins/timePlugin.h"
#include "plugins/inPlugin.h"
#include "plugins/flipPlugin.h"
#include "plugins/countPlugin.h"
#include "plugins/becomePlugin.h"
#include "plugins/quizPlugin.h"
#include "plugins/morningtonCrescentPlugin.h"
#include "plugins/ircExtraPlugin.h"
#include "plugins/aliasPlugin.h"

#include "stringhashtable.h"

class channel
{
	private:
		vector< plugin* > plugins;
		stringhashtable< plugin* > commandTable;
	public:
		channel()
		{
			// include only per-channel plugins here
			plugins.push_back(new replyPlugin());
			plugins.push_back(new timePlugin());
			plugins.push_back(new flipPlugin());
			plugins.push_back(new countPlugin());
			plugins.push_back(new quizPlugin());
			plugins.push_back(new morningtonCrescentPlugin());
			plugins.push_back(new ircExtraPlugin());
			//alias plugin should go last to stop other commands being realiased.
			//new hashtable breaks this.
			plugins.push_back(new aliasPlugin());
			
		}
		void addPlugin(plugin* p);
		//handle commads for all plugins, returns 0 on not matched, 1 on matched
		int handleCommand(string nick, string channel, vector<string> words);
		//handle messages for all plugins, returns 0 on not matched, 1 on matched
		int handleMessage(string nick, string channel, vector<string> words);
		//allows each plugin to run something each tick.  Returns the number of plugins that did something.
		int doTick();
		//allows each plugin to check the command line arguments and use them as they see fit.
		int startupOptions(vector<string> args);
};

#endif
