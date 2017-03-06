#ifndef __LEAGUE__
#define __LEAGUE__
#include <iostream>
using namespace std;
#include <string>
#define MAX_ROUNDS 30
#define NUM_TEAM 16
#define MATCHES_PER_ROUND 2
#define MAX_TEAM_NAME  32
#define MAX_FILENAME 40
#define NEG_INF -500
#define NotYetLabel 99
#define MAX_RECORDS 100
typedef struct
{
	int played;
	int won;
	int drawn;
	int lost;
	int gf;
	int ga;
	int gd;
	int pts;
}TeamStats;

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
		void getOppenentTeamName(char* myTeam, char*oppenetTeam);
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
		Team() :rank(0), ReserveTeamPts(0)
		{
			strcpy(teamName, "\0");
			memset(&MainStats, 0, sizeof(TeamStats));
			memset(&SubStats, 0, sizeof(TeamStats));
		}
		Team(char* tName) :rank(0), ReserveTeamPts(0)
		{
			strcpy(teamName,tName);
			memset(&MainStats, 0, sizeof(TeamStats));
			memset(&SubStats, 0, sizeof(TeamStats));
		}
		void setTeamName(char* tName);
		void setMatchByRound(int round, char* ht, char* at, int hg, int ag);
		void setRank(int rnk);
		void setReserveTeamPts(int RTPts);
		int getReserveTeamPts()const;

		void getMatchScoreByTwoTeam(char *tname1, char *tname2, int &gf, int &ga, int &match_num);
		char * getTeamName();
		void getOppenentName(int round, char* oppenentTeamName);
		void getTeamData(int &pl, int &wo, int &dr, int &lo, int &gf, int &ga, int &pts);
		void calcTeamData();

		int getPoints()const;
		int getSubPoints()const;
		int getGD()const;
		int getSubGD()const;
		int getGF()const;
		int getSubGF()const;
		void dispTeamData();	
		void dispTeamSubData();

		void updateSubStats(int gf, int ga);
		void dispTeamSchedule();
		void dispRoundMatch(int round);
		
	private:
		char teamName[MAX_TEAM_NAME];
		//string players[30];
		Match tMatches[MAX_ROUNDS];
		int rank;
		int ReserveTeamPts;
		TeamStats MainStats;
		TeamStats SubStats;

};
class League
{
	public:
		League():cur_team_num(0)
		{
			for (int i = 0; i < NUM_TEAM; i++)
			{
				rank[i] = i + 1;
			}
		}
		bool existTeam(char *tname);
		void addTeam(char *tname);

		int getTeamID(char* tname);
		int getTeamNum();
		int getTeamPointsByID(int id);
		char* getTeamNameByID(int id);
		void setReserveTeamPtsByTeamName(char* tName, int RTPts);

		void setMatchByID(int id, int round, char* hteam, char*ateam, int gfor, int gagainst);
		void calcLeagueData();

		void sortLeagueGDGF();
		void sortLeagueCSL();
		void sortLeagueCSL_Simple();
		void updateSamePtsTeamSubStats();
		void returnRank2Team();

		void dispOneTeam(char* tname);
		void dispTable();
		void dispScheduleAll();
		void dispScheduleByRound(int rnd);
	private:
		Team allTeams[NUM_TEAM];
		int rank[NUM_TEAM];
		int cur_team_num;
};

#endif