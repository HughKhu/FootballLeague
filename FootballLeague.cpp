// FootballLeague.cpp : 定义控制台应用程序的入口点。
//

//#include "stdafx.h"

#include <fstream>
#include <streambuf>

#include "LEAGUE.h"



int main(int argc, char* argv[])
{
	int round, goal_for, goal_against,id;
	char home_team[MAX_TEAM_NAME], away_team[MAX_TEAM_NAME];
	char filename[MAX_FILENAME];
	cout<<"MainLeagueFilename = ";
	gets(filename);
	FILE *fp = fopen(filename, "r");
	if (!fp)
	{
		cout << "Error! Such File Dose Not Exist!" << endl;
		system("PAUSE");
		exit(1);
	}
	//FILE *fp = fopen("data_30.csv", "r");
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
	cout << "Main League Loading Finished" << endl;
	char hasReserveLeague;
	cout << "Load ReserveLeague?<Y/N>: ";
	hasReserveLeague = getchar();
	getchar();
	if (hasReserveLeague == 'Y')
	{
		cout << "ReserveLeagueFilename = ";
		gets(filename);
		FILE *fp = fopen(filename, "r");
		if (!fp)
		{
			cout << "Error! Such File Dose Not Exist!" << endl;
			system("PAUSE");
			exit(1);
		}
		League CSL_2;
		while (!feof(fp))
		{
			fscanf(fp, "%d,%[^,],%d,%d,%s\n", &round, home_team, &goal_for, &goal_against, away_team);
			//cout << round <<" "<<  home_team << " " << goal_for << " " << goal_against << " " <<  away_team << endl;
			if (!CSL_2.existTeam(home_team))
			{
				CSL_2.addTeam(home_team);
			}
			id = CSL_2.getTeamID(home_team);
			CSL_2.setMatchByID(id, round, home_team, away_team, goal_for, goal_against);

			if (!CSL_2.existTeam(away_team))
			{
				CSL_2.addTeam(away_team);
			}
			id = CSL_2.getTeamID(away_team);
			CSL_2.setMatchByID(id, round, home_team, away_team, goal_for, goal_against);
		}
		fclose(fp);
		cout << "Reserved League Loading Finished" << endl;
		CSL_2.calcLeagueData();
		//CSL_2.sortLeagueCSL_Simple();
		//CSL_2.sortLeagueCSL();
		CSL_2.sortLeagueGDGF();
		cout << "**********************-----Reserved League Table-----**************************" << endl;
		CSL_2.dispTable();
		for (int j = 0; j < CSL_2.getTeamNum(); j++)
		{
			CSL.setReserveTeamPtsByTeamName(CSL_2.getTeamNameByID(j),CSL_2.getTeamPointsByID(j));
		}
	}

	CSL.calcLeagueData();
	//CSL.dispTable();
	
	//CSL.sortLeagueCSL_Simple();
	CSL.sortLeagueCSL();
	//CSL.sortLeagueGDGF();
	cout << "***********************-----Main League Table-----*****************************" << endl;
	CSL.dispTable();
	
	CSL.dispScheduleByRound(1);
	//CSL.dispScheduleAll();
	CSL.dispOneTeam("富力");
	system("PAUSE");
	return 0;
}

