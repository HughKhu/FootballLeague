#include "LEAGUE.h"

typedef int(*cmpMethodTeam)(Team &t1, Team &t2);
int cmpTeamPtsGdGf(Team &t1, Team &t2)
{
	if (t1.getPoints() > t2.getPoints()) return 1;
	else if (t1.getPoints() < t2.getPoints()) return -1;
	else if (t1.getGD() > t2.getGD()) return 1;
	else if (t1.getGD() < t2.getGD()) return -1;
	else if (t1.getGF() > t2.getGF()) return 1;
	else if (t1.getGF() < t2.getGF()) return -1;
	else return 0;	
}
int cmpTeamCSL(Team &t1, Team &t2)
{
	if (t1.getPoints() > t2.getPoints()) return 1;
	else if (t1.getPoints() < t2.getPoints()) return -1;
	else if (t1.getSubPoints() > t2.getSubPoints()) return 1;
	else if (t1.getSubPoints() < t2.getSubPoints()) return -1;
	else if (t1.getSubGD() > t2.getSubGD()) return 1;
	else if (t1.getSubGD() < t2.getSubGD()) return -1;
	else if (t1.getSubGF() > t2.getSubGF()) return 1;
	else if (t1.getSubGF() < t2.getSubGF()) return -1;
	//reserve team pts
	else if (t1.getReserveTeamPts() > t2.getReserveTeamPts()) return 1;
	else if (t1.getReserveTeamPts() < t2.getReserveTeamPts()) return -1;
	else if (t1.getGD() > t2.getGD()) return 1;
	else if (t1.getGD() < t2.getGD()) return -1;
	else if (t1.getGF() > t2.getGF()) return 1;
	else if (t1.getGF() < t2.getGF()) return -1;
	else return 0;
}

void sortTeam(Team *&teams, int teamNum, int rnk[], cmpMethodTeam cmp)
{//排序索引1，teams[i]的排位是rnk[i]
 //按teams从大到小排位，teams最大者rnk为0
 //允许rnk相同
	for (int i = 0; i < teamNum; i++) 
	{
		rnk[i] = 0;
	}
	for (int i = 1; i < teamNum; i++)
	{
		for (int j = i - 1; j >= 0; j--)
		{
			int flag = cmp(teams[i], teams[j]);
			if (flag == 0) continue;
			else if (flag < 0)//新插入的teams[i]小，名次要加1
			{
				rnk[i]++;
			}
			else //新插入的大，前面比它小的，名次加1
			{
				rnk[j]++;
			}
		}
	}
}

void League::setDispOrder(int arr[], int num, int odr[])
{//排序索引2，排位为i的元素是arr[odr[i]]
 //按arr从小到达排位，arr最小者的序号存于odr[0]中
 //因odr的下标均不同，故arr元素无法并列
	for (int i = 0; i < num; i++){
		odr[i] = i;
	}
	//类似于插入排序，在插入比较的过程中不断地修改dispOrder数组  
	for (int i = 1; i < num; i++)
	{
		int j = i;
		int tmp = odr[i];
		while (j > 0 && arr[tmp] < arr[odr[j - 1]])//新插入的arr[tmp]小，要往前排
		{
			odr[j] = odr[j - 1];
			j--;
		}
		odr[j] = tmp;
	}
}

void League::sortLeagueGDGF()
{
	Team * Teams = allTeams;
	sortTeam(Teams, cur_team_num, rank, cmpTeamPtsGdGf);
	//could have same rank.
	setDispOrder(rank, cur_team_num, dispOrder);
	for (int i = 0; i < cur_team_num; i++)
	{
		rank[i]++;
	}
	returnRank2Team();
}

void League::sortLeagueCSL_Simple()
{
	updateSamePtsTeamSubStats(false);//Do Not Disp subLeague
	Team * Teams = allTeams;
	sortTeam(Teams, cur_team_num, rank, cmpTeamCSL);
	//could have same rank.
	setDispOrder(rank,cur_team_num,dispOrder);
	for (int i = 0; i < cur_team_num; i++)
	{
		rank[i]++;
	}
	returnRank2Team();
}

void League::sortLeagueCSL()
{
	updateSamePtsTeamSubStats(true);//Disp subLeague
	Team * Teams = allTeams;
	sortTeam(Teams, cur_team_num, rank, cmpTeamCSL);
	//could have same rank.
	setDispOrder(rank, cur_team_num, dispOrder);
	for (int i = 0; i < cur_team_num; i++)
	{
		rank[i]++;
	}
	returnRank2Team();
}

void League::updateSamePtsTeamSubStats(bool DispSub)
{
	Team * Teams = allTeams;
	int *SameId = new int[cur_team_num];
	memset(SameId, -1, cur_team_num*sizeof(int));
	int SamePtsNum = 0;

	int *team_appear_id = new int[cur_team_num];
	memset(team_appear_id, -1, cur_team_num*sizeof(int));
	int team_appear_num = 0;

	bool appeared = false;
	for (int i = 0; i < cur_team_num; i++)
	{
		appeared = false;//if appeared, skip to next i;
		for (int j = 0; j < team_appear_num; j++)
		{
			if (i == team_appear_id[j]) { appeared = true; break; }
		}
		if (appeared) continue;

		memset(SameId, -1, cur_team_num*sizeof(int));
		SameId[0] = i;
		SamePtsNum = 1;
		int cur_pts = Teams[i].getPoints();
		for (int j = i + 1; j < cur_team_num; j++)
		{
			if (cur_pts == Teams[j].getPoints())
			{
				SameId[SamePtsNum] = j;
				SamePtsNum++;
				team_appear_id[team_appear_num] = j;
				team_appear_num++;
			}
		}
		if (SamePtsNum > 1)
		{//update these teams which have same total points
			if (DispSub)
			{
				cout << "--------------------" << endl;
			}
			int gfor, gagainst, f;
			for (int j = 0; j < SamePtsNum; j++)
			{//get relative matches. 
				for (int k = 0; k < SamePtsNum; k++)
				{
					if (j == k)continue;
					Teams[SameId[j]].getMatchScoreByTwoTeam(Teams[SameId[j]].getTeamName(), \
						Teams[SameId[k]].getTeamName(), gfor, gagainst, f);
					if (f == 1)
					{
						Teams[SameId[j]].updateSubStats(gfor, gagainst);
						Teams[SameId[k]].updateSubStats(gagainst, gfor);
						if (DispSub)
						{
							cout << Teams[SameId[j]].getTeamName() << "\t" << gfor << "-" \
								<< gagainst << "\t" << Teams[SameId[k]].getTeamName() << endl;
						}
					}
				}
			}
			//end update these teams which have same total points
			//disp subLeagueTable
			if (DispSub)
			{
				cout << "--------------------" << endl;
				//sort subLeague
				int *subRnk = new int[SamePtsNum];
				Team* subTeams = new Team[SamePtsNum];
				for (int j = 0; j < SamePtsNum; j++)
				{
					subTeams[j] = Teams[SameId[j]];
				}
				sortTeam(subTeams, SamePtsNum, subRnk,cmpTeamCSL);
				int *subOdr = new int[SamePtsNum];
				setDispOrder(subRnk,SamePtsNum,subOdr);
				
				//disp subLeague
				cout << "***************************-----Sub League-----********************************" << endl;
				//cout << "Rank\t" << "Team\t" << "Played\t" << "Won\t" << "Drawn\t" << "Lost\t" << "GF\t" << "GA\t" << "GD\t" << "Points" << endl;
				cout << "Rank\t" << "Team\t" << "Played\t"  \
					<< "GF\t" << "GA\t" << "GD\t" << "Points\t" \
					<< "RtPts\t" << "allGD\t" << "allGF" << endl;
				for (int j = 0; j < SamePtsNum; j++)
				{
					cout << subRnk[subOdr[j]] + 1 << '\t';
					Teams[SameId[subOdr[j]]].dispTeamSubData();//rank may be removed.
				}
				delete[]subTeams;
				delete[]subRnk;
				delete[]subOdr;
				cout << "***************************--------------------********************************" << endl;
			}
			//end disp subLeagueTable
		}
	}	
	delete[]SameId;
	delete[]team_appear_id;
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
{
	rank = rnk;
}
void Team::setReserveTeamPts(int RTPts)
{
	ReserveTeamPts = RTPts;
}
int Team::getReserveTeamPts()const
{
	return ReserveTeamPts;
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
void Team::getOppenentName(int round, char *oppenentTeamName)
{
	tMatches[round - 1].getOppenentTeamName(teamName, oppenentTeamName);
}
void Team::getTeamData(int &pl, int &wo, int &dr, int &lo, int &gf, int &ga, int &pts)
{
	pl = MainStats.played;
	wo = MainStats.won;
	dr = MainStats.drawn;
	lo = MainStats.lost;
	gf = MainStats.gf;
	ga = MainStats.ga;
	pts = MainStats.pts;
}
void Team::calcTeamData()
{//calculate Team Data except for rank.
	//memset(&MainStats, 0, sizeof(TeamStats));
	int gf, ga; bool f;
	for (int i = 0; i < MAX_ROUNDS; i++)
	{
		gf = 0; ga = 0;
		tMatches[i].getMatchScore(teamName, gf, ga, f);
		if (!f)
			continue;
		else
		{
			MainStats.gf += gf;
			MainStats.ga += ga;
			MainStats.played++;
			if (gf > ga){ MainStats.pts += 3; MainStats.won++; }
			else if (gf == ga) { MainStats.pts += 1; MainStats.drawn++; }
			else { MainStats.lost++; }
		}
	}
	MainStats.gd = MainStats.gf - MainStats.ga;
}

int Team::getPoints()const
{
	return MainStats.pts;
}
int Team::getSubPoints()const
{
	return SubStats.pts;
}
int Team::getGD()const
{
	return MainStats.gd;
}
int Team::getSubGD()const
{
	return SubStats.gd;
}
int Team::getGF()const
{
	return MainStats.gf;
}
int Team::getSubGF()const
{
	return SubStats.gf;
}
void Team::dispTeamData()
{
	//cout<<"Rank\t"<<"Team\t"<<"Played\t"<<"Won\t"<<"Drawn\t"<<"Lost\t"<<"GF\t"<<"GA\t"<<"GD\t"<<"Points"<<endl;
	cout << rank << "\t" << teamName << "\t" << MainStats.played << "\t" << MainStats.won <<\
		"\t" << MainStats.drawn << "\t" << MainStats.lost << "\t" << MainStats.gf <<\
		"\t" << MainStats.ga << "\t" << MainStats.gd << "\t" << MainStats.pts << endl;

	//cslAllDataDisp
	//cout << rank << "\t" << teamName << "\t" << MainStats.played << "\t" << MainStats.won << \
	//	"/" << MainStats.drawn << "/" << MainStats.lost << "\t" << MainStats.gf << \
	//	"/" << MainStats.ga  << "\t" << MainStats.pts\
	//	<< "\t" << SubStats.won << "/" << SubStats.drawn << "/" << SubStats.lost \
	//	<< "\t" << SubStats.gf << "/" << SubStats.ga \
	//	<< "\t" << SubStats.pts << "\t" << ReserveTeamPts << endl;

}
void Team::dispTeamSubData()
{
	//cout << rank << '\t';
	//cout << teamName << "\t" << SubStats.played << "\t" << SubStats.won << \
	//	"\t" << SubStats.drawn << "\t" << SubStats.lost << "\t" << SubStats.gf << \
	//	"\t" << SubStats.ga << "\t" << SubStats.gd << "\t" << SubStats.pts << endl;
	cout << teamName << "\t" << SubStats.played << "\t" << SubStats.gf << \
		"\t" << SubStats.ga << "\t" << SubStats.gd << "\t" << SubStats.pts << \
		"\t" << ReserveTeamPts << "\t" << MainStats.gd << "\t" << MainStats.gf << endl;

}

void Team::updateSubStats(int gf, int ga)
{
	SubStats.played++;
	SubStats.gf += gf;
	SubStats.ga += ga;
	SubStats.gd += (gf - ga);
	if (gf > ga)
	{
		SubStats.won++;
		SubStats.pts += 3;
	}
	else if (gf == ga)
	{
		SubStats.drawn++;
		SubStats.pts += 1;
	}
	else{
		SubStats.lost++;
	}
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
bool League::existTeam(char *tname)
{
	for (int i = 0; i < cur_team_num; i++)
	{
		if (strcmp(allTeams[i].getTeamName(), tname) == 0)//same
			return true;
	}
	return false;
}
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

int League::getTeamID(char* tname)
{
	for (int i = 0; i < cur_team_num; i++)
	{
		if (strcmp(allTeams[i].getTeamName(), tname) == 0)//same
			return i;
	}
	return -1;
}

int League::getTeamNum()
{
	return cur_team_num;
}
int League::getTeamPointsByID(int id)
{
	return allTeams[id].getPoints();
}
char* League::getTeamNameByID(int id)
{
	return allTeams[id].getTeamName();
}
void League::setReserveTeamPtsByTeamName(char* tName, int RTPts)
{
	for (int i = 0; i < cur_team_num; i++)
	{
		if (strcmp(tName, allTeams[i].getTeamName()) == 0)
		{
			allTeams[i].setReserveTeamPts(RTPts);
			break;
		}
	}
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
	cout << "Rank\t" << "Team\t" << "Played\t" << "Won\t" << "Drawn\t" << "Lost\t"\
		<< "GF\t" << "GA\t" << "GD\t" << "Points" << endl;
	//cout << "Rank\t" << "Team\t" << "Played\t" << "W/D/L\t" << "GF/GA\t" << "Pts\t" \
	//	<< "sW/D/L\t" << "sGF/GA\t" << "subPts\t" << "resPts" << endl; //cslAllDataDisp
	for(int i = 0; i < cur_team_num; i++)
	{
		allTeams[dispOrder[i]].dispTeamData();
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