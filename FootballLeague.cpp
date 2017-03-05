// FootballLeague.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <fstream>
#include <streambuf>

#include "LEAGUE.h"



int _tmain(int argc, _TCHAR* argv[])
{
//	int rounds[MAX_RECORDS],goal_for[MAX_RECORDS],goal_against[MAX_RECORDS];
//	string home_team[MAX_RECORDS], away_team[MAX_RECORDS];

	int round, goal_for, goal_against;
	char home_team[MAX_TEAM_NAME], away_team[MAX_TEAM_NAME];
	char filename[40];
	cout<<"filename = ";
	gets(filename);
	FILE *fp = fopen(filename, "r");
	if (!fp)
	{
		cout << "Error! Such File Dose Not Exist!" << endl;
		system("PAUSE");
		exit(1);
	}
	//FILE *fp = fopen("data_30.csv", "r");
	int i = 0;
	int id;
	League CSL;
	while (!feof(fp))
	{
		fscanf(fp, "%d,%[^,],%d,%d,%s\n", &round, home_team, &goal_for, &goal_against, away_team);
		//cout << round <<" "<<  home_team << " " << goal_for << " " << goal_against << " " <<  away_team << endl;
		if (!CSL.existTeam(home_team))
		{
			CSL.addTeam(home_team);
		}
		id = CSL.getTeamID(home_team);
		CSL.setMatchByID(id, round, home_team, away_team, goal_for, goal_against);

		if (!CSL.existTeam(away_team))
		{
			CSL.addTeam(away_team);
		}
		id = CSL.getTeamID(away_team);
		CSL.setMatchByID(id, round, home_team, away_team, goal_for, goal_against);
	}
	fclose(fp);

	CSL.calcLeagueData();
	//CSL.dispTable();

	CSL.sortLeagueCSL();
	CSL.dispTable();

	CSL.sortLeagueGDGF();
	CSL.dispTable();
	
	CSL.dispScheduleByRound(1);//double,16 matches.need to be improved.
	//CSL.dispScheduleAll();

	//CSL.dispOneTeam("恒大");
	system("PAUSE");
	return 0;
}

