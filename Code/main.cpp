//Writen by Chet Michals, April 30th 2012. For use with parased Dota 2 Replay Files

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include "helper.h"

using namespace std;

int __cdecl main( int argc, char *argv[] )
{
	if( argc <= 1 )
	{
		printf( "RoshanInfo.exe ParasedDemo.txt\n" );
		exit( 0 );
	}

	string input;
	
	vector <RoshanDeaths> RoshList;

	vector <PlayerInfo> parsedPlayers;
	
	int TempTeamNumber;
	int TempKillTick;
	int TempAegisNumber;
	int TempAegisTick;

	bool AegisGrabed = 1;

	int linecounter = 0;

	//String Constants
	string RoshanFall ("type: CHAT_MESSAGE_ROSHAN_KILL");
	string AegisFind ("type: CHAT_MESSAGE_AEGIS");
	string GameInfo( "game_info {");
	
	//Converts the passed in filename into a string, and opens it
	stringstream FileNameStream;
	string FileName;
	FileNameStream << argv[ 1 ];
//	FileNameStream << "13777207.txt";
	FileName += FileNameStream.str();
	fstream myfile;
	myfile.open (FileName);

	//Parses the text
	while(myfile.good())
	{
		getline (myfile,input);
		linecounter++;

		//Looks for Roshan has fallen messages
		if ((RoshanFall.compare(input) == 0))
		{
			if (AegisGrabed == 0) //If Roshan has been killed again, but no one picked up the Aegis the last kill
			{
				RoshList.push_back(MakeRoshanStruct(TempTeamNumber,TempKillTick,TempAegisNumber,TempAegisTick,AegisGrabed));
			}
			TempTeamNumber = ReturnPlayerNumber(myfile);
			TempKillTick = ReturnTickNumber(myfile);
			AegisGrabed = 0;

		}

		//Looks for Aegis picked up messages
		else if ((AegisFind.compare(input) == 0))
		{
			TempAegisNumber = ReturnPlayerNumber(myfile);
			TempAegisTick = ReturnTickNumber(myfile);
			AegisGrabed = 1;
			RoshList.push_back(MakeRoshanStruct(TempTeamNumber,TempKillTick,TempAegisNumber,TempAegisTick,AegisGrabed));
		}

		//Looks for the list of heroes at the end of the file
		else if ((GameInfo.compare(input) == 0))
		{
			if (AegisGrabed == 0) //If Roshan has been killed, but no one picked up the Aegis
			{
				RoshList.push_back(MakeRoshanStruct(TempTeamNumber,TempKillTick,TempAegisNumber,TempAegisTick,AegisGrabed));
			}
			parsedPlayers = parsePlayers(myfile);
		}
		else if (linecounter%10000 == 0)
		{
			cout << "Processed " << linecounter << " lines." << endl;
		}
	}

	//Prints info to screen, and dumps data into comma delimited spreadsheet
	PrintInfo(RoshList,parsedPlayers);
	DumpInfo(RoshList,parsedPlayers,FileName);


	return 0;
}