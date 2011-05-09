
#include "morningtonCrescentPlugin.h"

vector<station> morningtonCrescentPlugin::stations;
vector<string> morningtonCrescentPlugin::rules;
list<player> morningtonCrescentPlugin::players;
list<player>::iterator morningtonCrescentPlugin::currentPlayer;
bool morningtonCrescentPlugin::gameRunning = false;
