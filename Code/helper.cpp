#include "helper.h"

int ReturnPlayerNumber(fstream &inputFile)
{
	string inputText;
	string PlayerID ("playerid_1: ");
	char number;

	while(inputFile.good())
	{
		getline(inputFile,inputText);
		if (inputText.compare(0,12,PlayerID) == 0)
		{
			number = inputText[12];
			return atoi(&number);
		}
	}

		return -1;
}

int ReturnTickNumber(fstream &inputFile)
{
	string inputText;
	string tick ("tick: ");
	char number[20];
	int numberLength;

	while(inputFile.good())
	{
		getline(inputFile,inputText);
		if (inputText.compare(0,6,tick) == 0)
		{
			numberLength = inputText.copy(number,20,6);
			number[numberLength] = '\0';
			return (atoi(number) - 1);
		}
	}

		return -1;
}

RoshanDeaths MakeRoshanStruct(int TeamNum, int KillTick, int AegisNum, int AegisTick, bool AegisGrabbed)
{
	RoshanDeaths newRosh;
	newRosh.TeamNumber = TeamNum;
	newRosh.KillTick = KillTick;
	if (AegisGrabbed == 1)
	{
		newRosh.AegisNumber = AegisNum;
		newRosh.AegisTick = AegisTick;
	}
	else
	{
		newRosh.AegisNumber = -1;
		newRosh.AegisTick = 0;
	}
	return newRosh;
}

vector <PlayerInfo> parsePlayers(fstream &input)
{
	vector <PlayerInfo> LocalParse;
	string inputText;
	string HeroNameStart("      hero_name: ");
	string PlayerNameStart("      player_name: ");

	char HeroName[256];
	char PlayerName[256];

	int HeroNameLength;
	int PlayerNameLength;

	while(input.good()) //Last parts of the file, so won't return vector till after the full file is read
	{
		getline(input,inputText);
		if (inputText.compare(0,17,HeroNameStart) == 0)
		{
			HeroNameLength = inputText.copy(HeroName,255,17);
			HeroName[HeroNameLength] = '\0';

			//Player Name is always after hero name
			getline(input,inputText);
			PlayerNameLength = inputText.copy(PlayerName,255,19);
			PlayerName[PlayerNameLength] = '\0';

			PlayerInfo Temp;
			Temp.heroname = HeroName;
			Temp.playername = PlayerName;

			LocalParse.push_back(Temp);
		}
	}
	return LocalParse;
}

void PrintInfo(vector <RoshanDeaths> RoshList, vector <PlayerInfo> parsedPlayers)
{
	cout.precision(6);
	system("cls");
	int temp;
	float ReplayTimeKill;
	float ReplayTimeGrab;
	float TimeDiff;
	cout << "Number of times Roshan Killed: " << RoshList.size() << endl << endl;
	temp = RoshList.size();
	int AegisNumber;

	for (int i = 0; i < temp; i++)
	{
		cout << i+1 << ": "<< endl << "Roshan was killed by the";
		if (RoshList[i].TeamNumber == 2)
		{
			cout << " the Radiant, ";
		}
		else
		{
			cout << " the Dire, ";
		}

		ReplayTimeKill = (float)(RoshList[i].KillTick) / 30;
		cout << ReplayTimeKill << " seconds into the replay." << endl;
		ReplayTimeGrab = (float)(RoshList[i].AegisTick) / 30;
		if (RoshList[i].AegisTick == 0)
		{
			cout << "The Aegis was denied or never grabed." << endl;
		}
		else
		{
			AegisNumber = RoshList[i].AegisNumber;
			TimeDiff = (ReplayTimeGrab - ReplayTimeKill);
			cout << TimeDiff << " seconds later, the Aegis was taken by " << parsedPlayers[AegisNumber].heroname << endl;
			if (AegisNumber < 5)
			{
				cout << "of the Radiant." << endl;
			}
			else
			{
				cout << "of the Dire." << endl;
			}
		}
		cout << endl;
	}
}

void DumpInfo(vector <RoshanDeaths> RoshList, vector <PlayerInfo> parsedPlayers, string FileName)
{
	int TeamNum;
	ofstream TextFile;
	stringstream FileNameStream;	
	FileName += ".csv";
	TextFile.open(FileName);
	//Team 0 is Rad, Team 1 is Dire. 30 ticks happen per second. 
	if (RoshList.size() != 0)
	{
		TextFile << "Team Number, Roshan Kill Tick Number, Hero who grabbed Aegis, Hero Team, Aegis Grab Tick." << endl;
	}
	int RoshListSize = RoshList.size();
	for (int j = 0; j < RoshListSize; j++)
	{
		if (5 > (RoshList[j].AegisNumber))
		{
			TeamNum = 0;
		}
		else
		{
			TeamNum = 1;
		}
		FileNameStream.str("");
		if (RoshList[j].AegisNumber != -1)
		{
		FileNameStream << RoshList[j].TeamNumber-2 << ", " << RoshList[j].KillTick << ", " <<  parsedPlayers[RoshList[j].AegisNumber].heroname << ", " <<  TeamNum << ", " <<  RoshList[j].AegisTick;
		}
		else
		{
			FileNameStream << RoshList[j].TeamNumber-2 << ", " << RoshList[j].KillTick << ", " << "Denied" << ", " <<  TeamNum << ", " <<  RoshList[j].AegisTick;
		}
		TextFile << FileNameStream.str() << endl;
	}
	TextFile.close();

}