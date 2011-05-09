#ifndef PLUGINS_H
#define PLUGINS_H
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

class plugins
{
	private:
		vector<plugin*> pluginList;
	public:
		plugins()
		{
			pluginList.push_back(new filePlugin);
			pluginList.push_back(new replyPlugin);
			pluginList.push_back(new timePlugin);
			pluginList.push_back(new inPlugin);
			pluginList.push_back(new flipPlugin);
			pluginList.push_back(new countPlugin);
			pluginList.push_back(new becomePlugin);
			pluginList.push_back(new quizPlugin);
			pluginList.push_back(new morningtonCrescentPlugin);
			pluginList.push_back(new ircExtraPlugin);
			//alias plugin should go last to stop other commands being realiased.
			pluginList.push_back(new aliasPlugin);
		}
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
