#ifndef ALIASPLUGIN_H
#define ALIASPLUGIN_H

#include "../plugin.h"
#include "../stringhashtable.h"

using namespace std;

class aliasPlugin:public plugin
{
	private:
		static stringhashtable< vector< string > > aliases;
	public:
		int handleCommand(string nick, string channel, vector<string> words)
		{
			string reply = "";
			if(nick != "")
			{
				reply += nick + ": ";
			}
			if(words.at(0) == "alias")
			{
				if(words.size() < 3)
				{
					ircNet.sendMsg(channel, reply + "Format: !alias name command");
				}
				else
				{
					vector<string> command = words;
					command.erase(command.begin(), command.begin()+2);
					addAlias(words.at(1), command);
					ircNet.sendMsg(channel, reply + "Aliased '" + words.at(1) + "' to '" + stringUtils::joinWords(command) + "'.");
				}
			}
			else if(words.at(0) == "unalias")
			{
				if(words.size() < 2)
				{
					ircNet.sendMsg(channel, reply + "Format: !unalias name");
				}
				else
				{
					string result = showAlias(words.at(1));
					if(deleteAlias(words.at(1)))
					{
						
						ircNet.sendMsg(channel, reply + "Unaliased '" + words.at(1) + "', was aliased to '" + result + "'.");
					}
					else
					{
						ircNet.sendMsg(channel, reply + "There was a problem unaliasing '" + words.at(1) + "'.");
					}
				}
			}
			else if(words.at(0) == "showalias")
			{
				if(words.size() < 2)
				{
					ircNet.sendMsg(channel, reply + "Format: !showalias name");
				}
				else
				{
					string result = showAlias(words.at(1));
					if(result != "")
					{
						ircNet.sendMsg(channel, reply + "'" + words.at(1) + "' is aliased to '" + showAlias(words.at(1)) + "'.");
					}
					else
					{
						ircNet.sendMsg(channel, reply + "Alias '" + words.at(1) + "' not found.");
					}
				}
			}
			else if (aliases.contains(words[0])) {
				handleAllCommands(nick, channel, aliases.get(words[0]));
				return 1;
			}
			return 0;
		}

		static bool addAlias(string aliasName, vector<string> command)
		{
			if (!aliases.contains(aliasName)) {
				aliases.insert(aliasName, command);
				return true;
			} else return false;
		}

		static bool deleteAlias(string aliasName)
		{
			if (aliases.contains(aliasName)) {
				aliases.remove(aliasName);
				return true;
			} else return false;
		}

		static string showAlias(string aliasName)
		{
			if (aliases.contains(aliasName)) 
				return stringUtils::joinWords(aliases.get(aliasName));
			else
				return "";
		}
};

#endif
