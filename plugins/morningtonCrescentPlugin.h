#ifndef MORNINGTONCRESCENTPLUGIN_H
#define MORNINGTONCRESCENTPLUGIN_H

#include "../plugin.h"
#include "timePlugin.h"
#include "filePlugin.h"
#include "../stringUtils.h"
#include <math.h>
#include <cstdlib>
#include <list>


struct station
{
	int id;
	string name;
};

struct player
{
	string nick;
	int lastStation;
	int moves; // no. moves made
	int correct; // no. correct moves made
};

class morningtonCrescentPlugin:public plugin
{
	private:
		static vector<station> stations;
		vector<string> rules;
		list<player> players;
		list<player>::iterator currentPlayer;
		bool gameRunning;
	public:
		morningtonCrescentPlugin() {
			gameRunning = false;
		}

		int countVowels(string s) {
			s = stringUtils::toLower(s);
			int c = 0;
			string::iterator i = s.begin();
			while (i != s.end()) {
				if (
					*i == 'a' || *i == 'e' || *i == 'i' || *i == 'o' || *i == 'u'
				) c++;
				++i;
			}
			return c;
		}

		string oneArgument(vector<string> words) { // stringUtils also does this, may change
			string onearg = "";
			vector<string>::iterator i = words.begin();
			i++;
			while (i != words.end()) {
				onearg = onearg + *i;
				++i;
				if (i != words.end()) {
					onearg = onearg + " ";
				}
			}
			return onearg;
		}

		// the loading code is very similar to quizPlugin.h
		string loadStations()
		{
			string buffer;

			stations.clear();
			stringstream stationFile;
			struct station result;
			filePlugin::loadFile("stations", stationFile);
			int id = 0;
			while(getline(stationFile, buffer))
			{
				result.id = ++id;
				result.name = buffer;
				stations.push_back(result);
			}

			rules.clear();
			stringstream rulesFile;
			filePlugin::loadFile("rules", rulesFile);
			while(getline(rulesFile, buffer))
			{
				rules.push_back(buffer);
			}

			return "Finished loading " + stringUtils::toString(id) + " stations.";
		}

		string getStationById(int stationNum)
		{
			if(stationNum > stations.size() || stationNum <= 0)
			{
				return "No such station.";
			}
			else
			{
				return stations.at(stationNum-1).name;
			}
		}

		int getStationByName(string name)
		{
			// Linear search
			// TODO: make this more efficient
			vector<station>::iterator i = stations.begin();
			cout << name << endl;
			while (i != stations.end()) {
				if (stringUtils::toLower(i->name) == stringUtils::toLower(name)) {
					return i->id;
				}
				++i;
			}
			return -1;
		}

		int handleCommand(string nick, string channel, vector<string> words)
		{
			string reply = "";
			if(nick != "")
			{
				reply += nick + ": ";
			}

			if(words.at(0) == "players") {
				if (!gameRunning) {
					ircNet.sendMsg(channel, reply + "There's no game running now.");
					return 1;
				}
				string playerList = "";
				list<player>::iterator i = players.begin();
				while (i != players.end()) {
					playerList = playerList + i->nick;
					++i;
					if (i != players.end()) {
						playerList = playerList + ", ";
					}
				}
				ircNet.sendMsg(channel, reply + "Players in this game (in move order): <" + playerList + ">");
				ircNet.sendMsg(channel, reply + currentPlayer->nick + " is next to move.");
				return 1;
				
			}
			else if(words.at(0) == "move")
			{
				if (!gameRunning) {
					ircNet.sendMsg(channel, reply + "There's no game running now.");
					return 1;
				}
				list<player>::iterator pi = players.begin();
				// Does the player exist?
				while (pi != players.end()) {
					if (pi->nick == nick) break;
					++pi;
				}
				// No, they do not
				if (pi == players.end()) {
					// Add the new player and give them their first turn
					addPlayer(nick);
					currentPlayer--;
					ircNet.sendMsg(channel, "Welcome to the game, " + nick + ".");
				}

				if (currentPlayer->nick == nick) {
					// It is currentPlayer's turn
					if (words.size() < 2) {
						ircNet.sendMsg(channel, reply + "Please choose a station.");
						return 1;
					}
					// Find the station
					string stationName = oneArgument(words);
					if (getStationByName(stationName) != -1) {
						// is station valid?
						int vowelDifference = countVowels(getStationById(currentPlayer->lastStation))-countVowels(stationName);
						if ( (vowelDifference > 2 || vowelDifference < -2) && !(currentPlayer->moves == 0)) {
							// station invalid
							currentPlayer->correct = 0;
							if (rules.size() != 0) {
								int ruleNo = rand()%rules.size();
								ircNet.sendMsg(channel, reply + "Sorry, you can't reach " + stationName + " from " + getStationById(currentPlayer->lastStation) + ", because " + rules[ruleNo] + ".  Please move again.");
							} else {
								ircNet.sendMsg(channel, reply + "Sorry, you can't reach " + stationName + " from " + getStationById(currentPlayer->lastStation) + ".  Please move again.");
							}
							return 1;
						}
						if (stationName == "Mornington Crescent" && currentPlayer->correct < 3) {
							ircNet.sendMsg(channel, reply + "Sorry, you can't reach Mornington Crescent yet.  Please move again.");
							return 1;					}
						// Move
						currentPlayer->lastStation = getStationByName(stationName);
						currentPlayer->moves++;
						ircNet.sendMsg(channel, nick + " moves to " + stationName + ".");
						// Is this a winning station?
						if (stationName == "Mornington Crescent") {
							ircNet.sendMsg(channel, nick + " wins the game.");
							gameRunning = false;
							return 1;
						}
						currentPlayer->correct++;
						// Next player
						++currentPlayer;
						if (currentPlayer == players.end()) {
							currentPlayer = players.begin();
						}
						ircNet.sendMsg(channel, currentPlayer->nick + " to move now.");
						return 1;
					} else {
						ircNet.sendMsg(channel, reply + "That's not a London Underground station, please move again.");
						return 1;
					}
				} else {
					ircNet.sendMsg(channel, reply + "It's not your turn!");
				}
			}
			else if(words.at(0) == "reloadstations")
			{
				ircNet.sendMsg(channel, reply + loadStations());
				return 1;
			}
			else if(words.at(0) == "startgame")
			{
				if (gameRunning) {
					ircNet.sendMsg(channel, reply + "A game is already running.");
					return 1;
				} else {
					players.clear();
					gameRunning = true;
					currentPlayer = players.begin();
					addPlayer(nick);
					currentPlayer = players.begin();
					ircNet.sendMsg(channel, reply + "Starting a new game.");
					ircNet.sendMsg(channel, currentPlayer->nick + " to move now.");
					return 1;
				}
			}
			else if(words.at(0) == "stopgame")
			{
				gameRunning = false;
				ircNet.sendMsg(channel, nick + ": Stopping game.");
			}
			return 0;
		}

		void addPlayer(string nick) {
			struct player newPlayer;
			newPlayer.nick = nick;
			newPlayer.lastStation = -1;
			newPlayer.moves = 0;
			newPlayer.correct = 0;
			players.insert(currentPlayer, newPlayer);
		}


		int handleMessage(string nick, string channel, vector<string> words)
		{
			return 0;
		}

		int startupOptions(vector<string> args)
		{
			return 0;
		}
};

#endif
