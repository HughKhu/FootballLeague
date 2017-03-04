#include "stdafx.h";
#include "LEAGUE.h"

typedef int(*pf)(Team &t1, Team &t2);
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

void swapTeam(Team &t1, Team &t2)
{
	Team tmp = t1;
	t1 = t2;
	t2 = tmp;
}

void sortTeam(Team *&teams, int teamNum, pf cmp)
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

void League::sortLeague()
{
	Team * Teams = allTeams;
	sortTeam(Teams, cur_team_num, cmpTeam);
	for (int i = 0; i < cur_team_num; i++)
	{
		disp_order[i] = i;
		rank[i] = i + 1;
		returnRank2Team();
	}
}


void swap(int &a, int &b)
{
	int tmp = a;
	a = b;
	b = tmp;
}
void sort2Array(int *src1, int *src2, int *Rindex, int lens)
{
	int *src_1 = new int[lens];
	int *src_2 = new int[lens];
	for (int i = 0; i < lens; i++)
	{
		src_1[i] = src1[i];
		src_2[i] = src2[i];
		Rindex[i] = i;
	}
	for (int i = 0; i < lens; i++)
	{
		for (int j = 1; j < lens - i; j++)
		{
			if (src_1[i] < src_1[i + j])
			{
				swap(src_1[i], src_1[i + j]);
				swap(src_2[i], src_2[i + j]);
				swap(Rindex[i], Rindex[i + j]);
			}
			else if (src_1[i] == src_1[i + j] && src_2[i] < src_2[i + j])
			{
				swap(src_1[i], src_1[i + j]);
				swap(src_2[i], src_2[i + j]);
				swap(Rindex[i], Rindex[i + j]);
			}
		}
	}
	delete[] src_1;
	delete[] src_2;
}
void sort3Array(int *src1, int *src2, int *src3, int *Rindex, int lens)
{
	int *src_1 = new int[lens];
	int *src_2 = new int[lens];
	int *src_3 = new int[lens];
	for (int i = 0; i < lens; i++)
	{
		src_1[i] = src1[i];
		src_2[i] = src2[i];
		src_3[i] = src3[i];
		Rindex[i] = i;
	}
	for (int i = 0; i < lens; i++)
	{
		for (int j = 1; j < lens - i; j++)
		{
			if (src_1[i] < src_1[i + j])
			{
				swap(src_1[i], src_1[i + j]);
				swap(src_2[i], src_2[i + j]);
				swap(Rindex[i], Rindex[i + j]);
			}
			else if (src_1[i] == src_1[i + j])
			{
				if (src_2[i] < src_2[i + j])
				{
					swap(src_1[i], src_1[i + j]);
					swap(src_2[i], src_2[i + j]);
					swap(Rindex[i], Rindex[i + j]);
				}
				else if (src_2[i] == src_2[i + j] && src_3[i] < src_3[i + j])
				{
					swap(src_1[i], src_1[i + j]);
					swap(src_2[i], src_2[i + j]);
					swap(Rindex[i], Rindex[i + j]);
				}
			}
		}
	}
	delete[] src_1;
	delete[] src_2;
}
int points301(int gf, int ga)
{
	if (gf > ga) return 3;
	else if (gf == ga) return 1;
	else return 0;
}
void updateWonDrawnLost(int gf, int ga, int &wo, int &dr, int &lo)
{
	if (gf > ga) wo++;
	else if (gf == ga) dr++;
	else lo++;
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
//Rank Setting is to be improved.
void Team::setRank(int rnk)
{//Rank Setting is to be improved.
	rank = rnk;
}
//calculate Team Data except for rank.
void Team::calcTeamData()
{
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
//calculate all teams' Data.except for rank.
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
void League::calcRankRough()
{
	int points[NUM_TEAM], max = 0;
	for (int i = 0; i<cur_team_num; i++)
	{
		points[i] = allTeams[i].getPoints();
		if (points[i]>max) max = points[i];
	}
	//sort, record allTeams[i]'s rank in rank[i];
	int *b = new int[max + 1];
	memset(b, 0, (max + 1)*sizeof(int));
	int *c = new int[max + 1];
	memset(c, 0, (max + 1)*sizeof(int));
	memset(rank, 0, cur_team_num*sizeof(int));//
	for (int i = 0; i<cur_team_num; i++)
	{
		b[points[i]]++;
	}
	c[max] = b[max];
	for (int i = max - 1; i >= 0; i--)
	{
		c[i] = c[i + 1] + b[i];
	}
	for (int i = 0; i<cur_team_num; i++)
	{
		rank[i] = c[points[i]] + 1 - b[points[i]];
	}
	delete[]b;
	delete[]c;
	returnRank2Team();
	calcDispOrder();
}
void League::calcRankRefineByGDGF()
{
	// calculate rank for teams with same points.
	int count;
	int team_same_id[NUM_TEAM];
	for (int rnk = 1; rnk < cur_team_num + 1; rnk++)
	{
		count = 0;
		for (int i = 0; i < cur_team_num; i++)
		{
			if (rank[i] == rnk)
			{
				team_same_id[count] = i;
				count++;
			}
		}
		if (count > 1)
		{
			//team_same_id 0~count-1
			int *gf = new int[count];
			int *gd = new int[count];
			int *gdgf_index = new int[count];
			for (int j = 0; j < count; j++)
			{
				gd[j] = allTeams[team_same_id[j]].getGD();
				gf[j] = allTeams[team_same_id[j]].getGF();
			}
			//sort gdgf[j]-> gdgf_index[j]
			sort2Array(gd, gf, gdgf_index, count);
			//If gd,gf are the same. I don't compare continue.
			for (int j = 0; j < count; j++)
			{
				rank[team_same_id[gdgf_index[j]]] = rnk + j;
			}

			delete[] gf;
			delete[] gd;
			delete[] gdgf_index;
		}
		memset(team_same_id, 0, cur_team_num*sizeof(int));
	}
	returnRank2Team();
	calcDispOrder();
}
void League::calcRankRefineByCSL()
{//Need to be improved using Points of Reserves team.
	// calculate rank for teams with same points.
	int count;
	int team_same_id[NUM_TEAM];
	for (int rnk = 1; rnk < cur_team_num + 1; rnk++)
	{
		count = 0;
		for (int i = 0; i < cur_team_num; i++)
		{
			if (rank[i] == rnk)
			{
				team_same_id[count] = i;
				count++;
			}
		}
		//if count == 1,continue circulation for.
		if (count > 1)
		{//team_same_id, record id of teams whose total points are the same.
			//get teams name.
			char **TeamNameMat = new char *[count];
			for (int i = 0; i < count; i++)
			{
				TeamNameMat[i] = allTeams[team_same_id[i]].getTeamName();
			}
			int gfor, gagainst, f;
			int *pts = new int[count];
			memset(pts, 0, count*sizeof(int));//
			int *gf = new int[count];
			memset(gf, 0, count*sizeof(int));//
			int *ga = new int[count];
			memset(ga, 0, count*sizeof(int));//
			int *played = new int[count];
			memset(played, 0, count*sizeof(int));//
			int *won = new int[count];
			memset(won, 0, count*sizeof(int));//
			int *drawn = new int[count];
			memset(drawn, 0, count*sizeof(int));//
			int *lost = new int[count];
			memset(lost, 0, count*sizeof(int));//

			cout << "--------------------" << endl;
			for (int i = 0; i < count; i++)
			{//get relative matches. 
				for (int j = 0; j < count; j++)
				{
					if (i == j)continue;
					allTeams[team_same_id[i]].getMatchScoreByTwoTeam(TeamNameMat[i], TeamNameMat[j], gfor, gagainst, f);
					if (f == 1)
					{//f: match num; it may be larger than 1 in Korean Classic League.
						//Because it takes more than one match in a team's home against the same opponent.
						pts[i] += points301(gfor, gagainst);
						pts[j] += points301(gagainst, gfor);
						played[i]++;
						played[j]++;
						updateWonDrawnLost(gfor, gagainst, won[i], drawn[i], lost[i]);
						updateWonDrawnLost(gagainst, gfor, won[j], drawn[j], lost[j]);
						gf[i] += gfor;
						gf[j] += gagainst;
						ga[i] += gagainst;
						ga[j] += gfor;
						//disp one match
						cout << TeamNameMat[i] << "\t" << gfor << "-" << gagainst << "\t" << TeamNameMat[j] << endl;
					}
				}
			}
			cout << "--------------------" << endl;
			int *gd = new int[count];
			for (int i = 0; i < count; i++)
			{
				gd[i] = gf[i] - ga[i];
			}
			//sort subLeague
			int *index = new int[count]; 
			sort3Array(pts, gd, gf, index, count); 
			//If pts,gd,gf all are the same.Compare points of Reserves team, then GD,GF of all the matches.
			//In this function, I don't compare continue.
			for (int j = 0; j < count; j++)
			{
				rank[team_same_id[index[j]]] = rnk + j;
			}
			//disp subLeague
			cout << "***************************-----Sub League-----********************************" << endl;
			cout << "Rank\t" << "Team\t" << "Played\t" << "Won\t" << "Drawn\t" << "Lost\t" << "GF\t" << "GA\t" << "GD\t" << "Points" << endl;
			for (int i = 0; i < count; i++)
			{
				allTeams[team_same_id[index[i]]].getTeamName();
				cout << i + 1 << '\t';
				cout << TeamNameMat[index[i]] << "\t" << played[index[i]] << "\t" << won[index[i]] << "\t" << drawn[index[i]] << "\t" \
					<< lost[index[i]] << "\t" << gf[index[i]] << "\t" << ga[index[i]] << "\t" << gd[index[i]] << "\t" \
					<< pts[index[i]] << endl;
			}
			cout << "***************************--------------------********************************" << endl;
			delete[] TeamNameMat;
			delete[] pts;
			delete[] gf;
			delete[] ga;
			delete[] gd;
			delete[] index;
			delete[] won;
			delete[] drawn;
			delete[] lost;
		}
		//above: more than 2 teams have same points
		memset(team_same_id, 0, cur_team_num*sizeof(int));//
	}
	returnRank2Team();
	calcDispOrder();
}
void League::returnRank2Team()
{
	for (int i = 0; i<cur_team_num; i++)
	{
		allTeams[i].setRank(rank[i]);
	}
}
void League::calcDispOrder()
{
	for (int i = 0; i < cur_team_num; i++)
	{
		disp_order[i] = -1;
	}
	for (int i = 0; i < cur_team_num; i++)
	{//rank[i] : 1~16
		if (rank[i] < 1 || rank[i]>16)
		{
			cout << "league.rank[i] error!" << endl;
			break;
		}
		disp_order[rank[i] - 1] = i;
	}
}
void League::dispRank()
{
	for (int i = 0; i < cur_team_num; i++)
	{
		cout << rank[i] << " ";
	}
	cout << endl;
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
		if (disp_order[i] < 0 || disp_order[i] > NUM_TEAM)
		{ cout << i + 1 << " rank error! Team rank repeat and No team ranks " << i + 1 << endl; continue; }
		allTeams[disp_order[i]].dispTeamData();
	}
	cout << "***************************--------------------********************************" << endl;
	/*for (int i = 0; i < cur_team_num; i++)
	{
	allTeams[i].dispTeamData();
	}*/
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
	for (int i = 0; i < cur_team_num; i++)
	{
		allTeams[i].dispRoundMatch(rnd);
	}
	cout << "--------------------" << endl;
}