#include "stdafx.h";
#include "LEAGUE.h"

typedef struct
{
	int played;
	int won;
	int drawn;
	int lost;
	int pts;
	int gf;
	int ga;
	int gd;
	//int allpts;
	int allgd;
	int allgf;
}TeamStats;
void dispStats(TeamStats stats)
{
	cout << stats.pts << " " << stats.gd << " " << stats.gf << " " <<\
		stats.allgd << " " << stats.allgf << endl;
}

typedef int(*cmpMethodStats)(TeamStats &s1, TeamStats &s2);
int cmpSamePts(TeamStats &s1, TeamStats &s2)
{
	if (s1.pts > s2.pts) return 1;
	else if (s1.pts < s2.pts) return -1;
	else if (s1.gd > s2.gd) return 1;
	else if (s1.gd < s2.gd) return -1;
	else if (s1.gf > s2.gf) return 1;
	else if (s1.gf < s2.gf) return -1;
	else if (s1.allgd > s2.allgd) return 1;
	else if (s1.allgd < s2.allgd) return -1;
	else if (s1.allgf > s2.allgf) return 1;
	else if (s1.allgf < s2.allgf) return -1;
	else return 0;
}

typedef int(*cmpMethodTeam)(Team &t1, Team &t2);
int cmpTeam(Team &t1, Team &t2)
{
	if (t1.getPoints() > t2.getPoints()) return 1;
	else if (t1.getPoints() < t2.getPoints()) return -1;
	else if (t1.getGD() > t2.getGD()) return 1;
	else if (t1.getGD() < t2.getGD()) return -1;
	else if (t1.getGF() > t2.getGF()) return 1;
	else if (t1.getGF() < t2.getGF()) return -1;
	else return 0;	
}
int cmpTeam_onlyPts(Team &t1, Team &t2)
{
	if (t1.getPoints() > t2.getPoints()) return 1;
	else if (t1.getPoints() < t2.getPoints()) return -1;
	else return 0;
}

void swapTeam(Team &t1, Team &t2)
{
	Team tmp = t1;
	t1 = t2;
	t2 = tmp;
}
void swapStats(TeamStats &s1, TeamStats &s2)
{
	TeamStats tmp = s1;
	s1 = s2;
	s2 = tmp;
}
void swap(int &a, int &b)
{
	int tmp = a;
	a = b;
	b = tmp;
}

void sortTeam(Team *&teams, int teamNum, cmpMethodTeam cmp)
{
	for (int i = 0; i < teamNum - 1; i++) // -1 or not does not influence.
	{
		for (int j = 0; j + 1 < teamNum - i; j++)
		{
			if (cmp(teams[j], teams[j+1]) < 0)
			{
				swapTeam(teams[j], teams[j+1]);
			}
		}
	}
}
void sortTeamSamePts(TeamStats* &StatsSet, int *index, int SamePtsNum,cmpMethodStats cmp)
{
	int lens = SamePtsNum;
	for (int i = 0; i < lens; i++)
	{
		index[i] = i;
	}
	for (int i = 0; i < lens - 1; i++) // -1 or not does not influence.
	{
		for (int j = 0; j + 1 < lens - i; j++)
		{
			if (cmp(StatsSet[j], StatsSet[j + 1]) < 0)
			{
				swapStats(StatsSet[j], StatsSet[j + 1]);
				swap(index[j], index[j + 1]);
			}
		}
	}
}

void League::sortLeagueCSL()
{
	Team * Teams = allTeams;
	sortTeam(Teams, cur_team_num, cmpTeam_onlyPts);
	for (int i = 0; i < cur_team_num; i++) rank[i] = i + 1;//rough rank
	int SamePtsIdFirst = 0, SamePtsNum = 0, cur_pts = 0, i = 0;
	while (i < cur_team_num)
	{
		cur_pts = Teams[i].getPoints();
		SamePtsIdFirst = i;
		SamePtsNum = 1;
		for (int j = i + 1; j < cur_team_num; j++)
		{
			if (Teams[j].getPoints() == cur_pts) SamePtsNum++;
			else break;
		}
		i = i + SamePtsNum;
		if (SamePtsNum > 1)
		{//cmp these teams
			Team * subTeams = Teams + SamePtsIdFirst;
			TeamStats * StatsSet = new TeamStats[SamePtsNum];
			memset(StatsSet, 0, SamePtsNum*sizeof(TeamStats));
			int gfor, gagainst, f;
			cout << "--------------------" << endl;
			for (int j = 0; j < SamePtsNum; j++)
			{//get relative matches. 
				for (int k = 0; k < SamePtsNum; k++)
				{
					if (j == k)continue;
					subTeams[j].getMatchScoreByTwoTeam(subTeams[j].getTeamName(), subTeams[k].getTeamName(), gfor, gagainst, f);
					if (f == 1)
					{//f: match num; it may be larger than 1 in Korean Classic League.
						//Because it takes more than one match in a team's home against the same opponent.
						StatsSet[j].played++;
						StatsSet[k].played++;
						StatsSet[j].gf += gfor;
						StatsSet[k].gf += gagainst;
						StatsSet[j].ga += gagainst;
						StatsSet[k].ga += gfor;
						if (gfor > gagainst){
							StatsSet[j].pts += 3; StatsSet[j].won++;
							StatsSet[k].lost++;}
						else if (gfor == gagainst) {
							StatsSet[j].pts += 1; StatsSet[j].drawn++;
							StatsSet[k].pts += 1; StatsSet[k].drawn++;}
						else {
							StatsSet[j].lost++;
							StatsSet[k].pts += 3; StatsSet[k].won++;}
						//disp one match
						cout << subTeams[j].getTeamName() << "\t" << gfor << "-" \
							<< gagainst << "\t" << subTeams[k].getTeamName() << endl;
					}
				}
			}
			cout << "--------------------" << endl;
			for (int j = 0; j < SamePtsNum; j++)
			{
				StatsSet[j].gd = StatsSet[j].gf - StatsSet[j].ga;
				StatsSet[j].allgd = subTeams[j].getGD();
				StatsSet[j].allgf = subTeams[j].getGF();
			}
			//sort subLeague
			int *index = new int[SamePtsNum];
			sortTeamSamePts(StatsSet, index, SamePtsNum, cmpSamePts);
			//If pts,gd,gf all are the same.Compare points of Reserves team, 
			//then GD,GF of all the matches.
			//In this function, I neglect comparison of Reserves teams.
			Team *tmpTeams = new Team[SamePtsNum];
			for (int j = 0; j < SamePtsNum; j++) tmpTeams[j] = subTeams[j];
			for (int j = 0; j < SamePtsNum; j++) subTeams[j] = tmpTeams[index[j]];
			//could have same rank.
			int pre_id = 0;
			for (int id = 1; id < SamePtsNum; id++)
			{
				if (cmpSamePts(StatsSet[pre_id], StatsSet[id]) == 0) 
					rank[SamePtsIdFirst + id] = SamePtsIdFirst + pre_id + 1;
				else {
					pre_id = id;
				}
			}
			//disp subLeague
			cout << "***************************-----Sub League-----********************************" << endl;
			cout << "Rank\t" << "Team\t" << "Played\t" << "Won\t" << "Drawn\t" << "Lost\t" << "GF\t" << "GA\t" << "GD\t" << "Points" << endl;
			for (int j = 0; j < SamePtsNum; j++)
			{
				cout << rank[SamePtsIdFirst+j] << '\t';
				cout << subTeams[j].getTeamName() << "\t" << StatsSet[j].played << "\t" << StatsSet[j].won << "\t" << StatsSet[j].drawn << "\t" \
					<< StatsSet[j].lost << "\t" << StatsSet[j].gf << "\t" << StatsSet[j].ga << "\t" << StatsSet[j].gd << "\t" \
					<< StatsSet[j].pts << endl;
			}
			cout << "***************************--------------------********************************" << endl;
		}
	}
	returnRank2Team();
}
void League::sortLeagueGDGF()
{
	Team * Teams = allTeams;
	sortTeam(Teams, cur_team_num, cmpTeam);
	//could have same rank.
	rank[0] = 1;
	int pre_i = 0;
	for (int i = 1; i < cur_team_num; i++)
	{
		if (cmpTeam(Teams[pre_i],Teams[i]) == 0) rank[i] = pre_i + 1;
		else {
			rank[i] = i + 1; pre_i = i;
		}
	}
	returnRank2Team();
}



//Match
void Match::setBothSidesTeamName(char* hTeam, char* aTeam)
{
	strcpy(homeTeam, hTeam);
	strcpy(awayTeam, aTeam);
}
void Match::setMatch(char* hTeam, char* aTeam, int hG, int aG)
{//to be improved to revise.
	if (tag)
	{
		cout << "Match already register.Refuse to change!" << endl;
	}
	else
	{
		strcpy(homeTeam, hTeam);
		strcpy(awayTeam, aTeam);
		hGoal = hG;
		aGoal = aG;
		if (hG == NotYetLabel && aG == NotYetLabel)
		{
			tag = false;
		}
		else { tag = true; }
	}
}
void Match::setMatch(int hG, int aG)
{
	if (tag)
	{
		cout << "Match already register.Refuse to change!" << endl;
	}
	else
	{
		hGoal = hG;
		aGoal = aG;
		tag = true;
	}
}
void Match::getMatchScore(char* tName, int &gf, int &ga, bool &flag)
{
	if (tag)
	{
		if (!strcmp(tName, homeTeam))//if same, strcmp return 0.
		{
			gf = hGoal; ga = aGoal; flag = true;
		}
		else if (!strcmp(tName, awayTeam))
		{
			gf = aGoal; ga = hGoal; flag = true;
		}
		else
		{
			cout << "Team " << tName << "dosen't exist in this match.Team Name Error!" << endl;
			flag = false;
		}
	}
	else{ flag = false; }
}
void Match::getMatchScore(char* tName1, char* tName2, int &gf, int &ga, bool &flag)
{
	ga = 0; gf = 0; flag = false;
	if (tag)
	{
		if (!strcmp(tName1, homeTeam) && !strcmp(tName2, awayTeam))//if same, strcmp return 0.
		{
			gf = hGoal; ga = aGoal; flag = true;
		}
		else
		{
			//cout << "Team " << tName1<<" and "<<tName2<< "dosen't exist in this match.Team Name Error!" << endl;
		}
	}
}
void Match::getMatch(char* hTeam, char* aTeam, int &gf, int &ga, bool &flag)
{
	strcpy(hTeam, homeTeam);
	strcpy(aTeam, awayTeam);
	gf = hGoal;
	ga = aGoal;
	flag = tag;
}
void Match::getOppenentTeamName(char* myTeam, char*oppenetTeam)
{
	if (strcmp(homeTeam, myTeam))//different
	{
		strcpy(oppenetTeam, homeTeam);
	}
	else
	{
		strcpy(oppenetTeam, awayTeam);
	
	}
}
void Match::dispMatch()
{
	if (tag == 1)
		//cout << homeTeam << "\t" << hGoal << "-" << aGoal << "\t" << awayTeam << "\ttag:\t" << tag << endl;
		cout << homeTeam << "\t" << hGoal << "-" << aGoal << "\t" << awayTeam << endl;
	else
		cout << homeTeam << "\t" << " " << "-" << " " << "\t" << awayTeam << endl;
}

//Team
void Team::setTeamName(char* tName)
{
	strcpy(teamName, tName);
}
void Team::setMatchByRound(int round, char* ht, char* at, int hg, int ag)
{
	if (round <1 || round >MAX_ROUNDS)
		cout << "Round overflow.Error!" << endl;
	else if (strcmp(ht, teamName) && strcmp(at, teamName))//strcmp equals return 0
	{
		cout << "Match between " << ht << " and " << at << " Shouldn't input into " << teamName << "'s Matches." << endl;
	}
	else
	{
		tMatches[round - 1].setMatch(ht, at, hg, ag);
	}
}
void Team::setRank(int rnk)
{//Rank Setting is to be improved.
	rank = rnk;
}
void Team::calcTeamData()
{//calculate Team Data except for rank.
	points = 0; played = 0; won = 0; drawn = 0; lost = 0; GF = 0; GA = 0;
	int gf, ga; bool f;
	for (int i = 0; i < MAX_ROUNDS; i++)
	{
		gf = 0; ga = 0;
		tMatches[i].getMatchScore(teamName, gf, ga, f);
		if (!f)
			continue;
		else
		{
			GF += gf;
			GA += ga;
			played++;
			if (gf > ga){ points += 3; won++; }
			else if (gf == ga) { points += 1; drawn++; }
			else { lost++; }
		}
	}
	GD = GF - GA;
}
void Team::getMatchScoreByTwoTeam(char *tname1, char *tname2, int &gf, int &ga, int &match_num)
{
	int gf_tmp, ga_tmp; bool flag;
	gf = 0; ga = 0; match_num = 0;
	for (int i = 0; i < MAX_ROUNDS; i++)
	{
		tMatches[i].getMatchScore(tname1, tname2, gf_tmp, ga_tmp, flag);
		if (flag)
		{
			gf += gf_tmp;
			ga += ga_tmp;
			match_num++;
		}
	}
}
char* Team::getTeamName()
{
	return teamName;
}
int Team::getPoints()const
{
	return points;
}
void Team::getTeamData(int &pl, int &wo, int &dr, int &lo, int &gf, int &ga, int &pts)
{
	pl = played;
	wo = won;
	dr = drawn;
	lo = lost;
	gf = GF;
	ga = GA;
	pts = points;
}
int Team::getGF()
{
	return GF;
}
int Team::getGD()
{
	return GD;
}
void Team::dispTeamData()
{
	//cout<<"Rank\t"<<"Team\t"<<"Played\t"<<"Won\t"<<"Drawn\t"<<"Lost\t"<<"GF\t"<<"GA\t"<<"GD\t"<<"points"<<endl;
	cout << rank << "\t" << teamName << "\t" << played << "\t" << won << "\t" << drawn << "\t" << lost << "\t"\
		<< GF << "\t" << GA << "\t" << GD << "\t" << points << endl;

}
void Team::dispTeamSchedule()
{
	cout << "----------------------------" << endl;
	cout << "       " <<teamName << " Schedule" << endl;
	cout << "----------------------------" << endl;
	for (int i = 0; i < MAX_ROUNDS; i++)
	{
		cout << i + 1 << "\t";
		tMatches[i].dispMatch();
	}
	cout << "----------------------------" << endl;
}
void Team::dispRoundMatch(int round)
{
	if (round <1 || round >MAX_ROUNDS){ cout << "cannot disp match of round " << round << ".round error!" << endl; }
	else
	{
		tMatches[round - 1].dispMatch();
	}
}
void Team::getOppenentName(int round, char *oppenentTeamName)
{
	tMatches[round - 1].getOppenentTeamName(teamName,oppenentTeamName);
}

//League
void League::addTeam(char *tname)
{
	if (existTeam(tname))
		cout << "Team " << tname << " exist. Cannot add the team." << endl;
	else
	{
		allTeams[cur_team_num].setTeamName(tname);
		cur_team_num++;
	}
}
bool League::existTeam(char *tname)
{
	for (int i = 0; i < cur_team_num; i++)
	{
		if (strcmp(allTeams[i].getTeamName(), tname) == 0)//same
			return true;
	}
	return false;
}
int League::getTeamID(char* tname)
{
	for (int i = 0; i < cur_team_num; i++)
	{
		if (strcmp(allTeams[i].getTeamName(), tname) == 0)//same
			return i;
	}
	return -1;
}
void League::setMatchByID(int id, int round, char* hteam, char*ateam, int gfor, int gagainst)
{
	allTeams[id].setMatchByRound(round, hteam, ateam, gfor, gagainst);
}
void League::calcLeagueData()
{//calculate all teams' Data.except for rank.
	for (int i = 0; i < cur_team_num; i++)
	{
		allTeams[i].calcTeamData();
		//should be delete later, only for test.
		rank[i] = i + 1;
		returnRank2Team();
	}
}
void League::returnRank2Team()
{
	for (int i = 0; i<cur_team_num; i++)
	{
		allTeams[i].setRank(rank[i]);
	}
}

void League::dispOneTeam(char* tname)
{
	if (existTeam(tname))
	{
		
		int id = getTeamID(tname);
		cout << "-------------------------------------------------------------------------------" << endl;
		cout << "Rank\t" << "Played\t" << "Team\t" << "Won\t" << "Drawn\t" << "Lost\t" << "GF\t" << "GA\t" << "GD\t" << "Points" << endl;
		allTeams[id].dispTeamData();
		cout << "-------------------------------------------------------------------------------" << endl;
		allTeams[id].dispTeamSchedule();
	}
	else { cout << "Team " << tname << " dosen't exist in this league" << endl; }
}
void League::dispTable()
{
	cout << "**************************-----League Table-----*******************************" << endl;
	cout << "Rank\t" << "Team\t" << "Played\t" << "Won\t" << "Drawn\t" << "Lost\t" << "GF\t" << "GA\t" << "GD\t" << "Points" << endl;
	for (int i = 0; i < cur_team_num; i++)
	{
		allTeams[i].dispTeamData();
	}
	cout << "***************************--------------------********************************" << endl;
}
void League::dispScheduleAll()
{
	for (int i = 0; i < cur_team_num; i++)
	{
		allTeams[i].dispTeamSchedule();
	}
}
void League::dispScheduleByRound(int rnd)
{
	if (rnd<1 || rnd>MAX_ROUNDS)
	{
		cout << "Cannot display matches of ROUND " << rnd << ". Round overflow." << endl;
		return;
	}
	cout << "------Round "<<rnd<<"-------" << endl;
	char *teamName = new char[MAX_TEAM_NAME];
	int team_appear_id[NUM_TEAM];
	int team_appear_num = 0;
	bool appeared = false;
	for (int i = 0; i < cur_team_num; i++)
	{
		appeared = false;
		for (int j = 0; j < team_appear_num; j++)
		{
			if (i == team_appear_id[j]) { appeared = true; break; }
		}
		if (appeared) continue;
		else
		{
			allTeams[i].dispRoundMatch(rnd);
			allTeams[i].getOppenentName(rnd,teamName);
			team_appear_id[team_appear_num] = getTeamID(teamName);
			team_appear_num++;
		}		
	}
	cout << "--------------------" << endl;
	delete[]teamName;
}