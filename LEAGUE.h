#ifndef __LEAGUE__
#define __LEAGUE__
#include <iostream>
using namespace std;
#include <string>
#define MAX_ROUNDS 30
#define NUM_TEAM 16
#define MATCHES_PER_ROUND 2
#define MAX_TEAM_NAME  32
#define NEG_INF -500
#define NotYetLabel 99
#define MAX_RECORDS 100

void swap(int &a, int &b);
void sort2Array(int *src1, int *src2, int *Rindex, int lens);
void sort3Array(int *src1, int *src2, int *src3, int *Rindex, int lens);
int points301(int gf, int ga);
void updateWonDrawnLost(int gf, int ga, int &wo, int &dr, int &lo);

class Match
{
	public:
		Match():tag(false),hGoal(0),aGoal(0)
		{
			strcpy(homeTeam, "\0");
			strcpy(awayTeam, "\0");
		}
		Match(char* hTeam,char* aTeam):tag(false),hGoal(0),aGoal(0)
		{
			strcpy(homeTeam,hTeam);
			strcpy(awayTeam, aTeam);
		}
		void setBothSidesTeamName(char* hTeam, char* aTeam);
		void setMatch(char* hTeam, char* aTeam, int hG, int aG);
		void setMatch(int hG, int aG);
		void getMatchScore(char* tName, int &gf, int &ga, bool &flag);
		void getMatchScore(char* tName1, char* tName2, int &gf, int &ga, bool &flag);
		void getMatch(char* hTeam, char* aTeam, int &gf, int &ga, bool &flag);
		void dispMatch();
	private:
		char homeTeam[MAX_TEAM_NAME];
		char awayTeam[MAX_TEAM_NAME];
		bool tag;
		int hGoal;
		int aGoal;
};

class Team
{
	public:
		Team() :rank(0), played(0), won(0), drawn(0), lost(0), GF(0), GA(0), GD(0), points(0)
		{ strcpy(teamName, "\0"); }
		Team(char* tName):rank(0),played(0),won(0),drawn(0),lost(0),GF(0),GA(0),GD(0),points(0)
		{strcpy(teamName,tName);}
		void setTeamName(char* tName);
		void setMatchByRound(int round, char* ht, char* at, int hg, int ag);
		//Rank Setting is to be improved.
		void setRank(int rnk);
		//calculate Team Data except for rank.
		void calcTeamData();
		void getMatchScoreByTwoTeam(char *tname1, char *tname2, int &gf, int &ga, int &match_num);
		char * getTeamName();
		int getPoints()const;
		void getTeamData(int &pl, int &wo, int &dr, int &lo, int &gf, int &ga, int &pts);
		int getGF();
		int getGD();
		void dispTeamData();
		void dispTeamSchedule();
		void dispRoundMatch(int round);
	private:
		char teamName[MAX_TEAM_NAME];
		//string players[30];
		Match tMatches[MAX_ROUNDS];
		int rank;
		int played;
		int won;
		int drawn;
		int lost;
		int GF;
		int GA;
		int GD;
		int points;
};
class League
{
	public:
		League():cur_team_num(0)
		{
			for (int i = 0; i < NUM_TEAM; i++)
			{
				rank[i] = i + 1;
				disp_order[i] = i;
			}
		}
		void addTeam(char *tname);
		bool existTeam(char *tname);
		int getTeamID(char* tname);
		void setMatchByID(int id, int round, char* hteam, char*ateam, int gfor, int gagainst);
		//calculate all teams' Data.except for rank.
		void calcLeagueData();
		void sortLeague();
		void calcRankRough();
		void calcRankRefineByGDGF();
		void calcRankRefineByCSL();
		void calcDispOrder();
		void returnRank2Team();
		void dispRank();
		void dispOneTeam(char* tname);
		void dispTable();
		void dispScheduleAll();
		void dispScheduleByRound(int rnd);
	private:
		Team allTeams[NUM_TEAM];
		int rank[NUM_TEAM];
		int disp_order[NUM_TEAM];//for Output order.
		int cur_team_num;
};

#endif