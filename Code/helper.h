#ifndef HELPER_H
#define HELPER_H
#pragma warning( disable : 4996 )

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

struct RoshanDeaths
{
	int TeamNumber;
	int KillTick;
	int AegisNumber;
	int AegisTick;
};

struct PlayerInfo
{
	string heroname;
	string playername;
};

int ReturnPlayerNumber(fstream &input);
int ReturnTickNumber(fstream &input);
RoshanDeaths MakeRoshanStruct(int TeamNum, int KillTick, int AegisNum, int AegisTick, bool AegisGrabbed);
vector <PlayerInfo> parsePlayers(fstream &input);

void PrintInfo(vector <RoshanDeaths> RoshList, vector <PlayerInfo> parsedPlayers);

void DumpInfo(vector <RoshanDeaths> RoshList, vector <PlayerInfo> parsedPlayers, string FileName);


#endif