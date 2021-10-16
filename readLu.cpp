#include <bits/stdc++.h>
#include "class.h"

using namespace std;

const int N = 14000; 
const int MinCT = 40, MaxBlk = 600, MaxDP = 720;
// const int NA = 207; 
string preData[N];
string preDataCrew[N];
const string crewFileA = "机组排班Data A-Crew.csv", crewFileB = "机组排班Data B-Crew.csv";
const string flightFileA = "机组排班Data A-Flight.csv", flightFileB = "机组排班Data B-Flight.csv";
const int flightNumA = 207, flightNumB = 13955;
const int crewNumA = 22, crewNumB = 466;	//	包括表头的一行，实际数据从 1 开始
bool isDataA = 1;

int sumMemberTakePlane;
int couldUseMemberNum[1000];

crewMember record[3000][500];
int alreadyUseNum;
crewMember memberData[N + 5];
struct crewMember *p;

fightData data[N + 5]; 

bool choose_Cmp(const crewMember & a, const crewMember & b) {
	if(a.isUsed != b.isUsed)
		return a.isUsed == false? true : false;
	if(a.level < b.level)	//	优先取 1，2
		return true;
	else if(a.level == b.level) {
		return !lessThen_end(a.endDate, a.endTime, b.endDate, b.endTime);	//	优先取结束时间更晚（离现在近）的，减少住酒店时间
	} else
		return false;
}

// bool FO_Cmp(const crewMember & a, const crewMember & b) {
// 	if(a.isUsed != b.isUsed)
// 		return a.isUsed == false? true : false;
// 	if(a.level > b.level)	//	优先取 3
// 		return true;
// 	else if(a.level == b.level) {
// 		return !lessThen_end(a.endDate, a.endTime, b.endDate, b.endTime);
// 	} else
// 		return false;
// }

pair<int, int> chooseCrew(crewMember memberData[]) {	//	排序飞行员，返回 {FO起始下标，CA起始下标}，找不到合法则为 -1
	int crewNum = isDataA? crewNumA : crewNumB;
	sort(memberData + 1, memberData + crewNum, choose_Cmp);
	int posCA = -1, posFO = -1;
	for(int i = 1; i < crewNum; ++i) {
		if(posFO == -1 && (memberData[i].level == 3 && !memberData[i].isUsed))
			posFO = i;
		if(posCA == -1 && (memberData[i].level != 3 && !memberData[i].isUsed))
			posCA = i;
		if(posCA > 0 && posFO > 0)
			break;
	}
	return make_pair(posFO, posCA);
}

// bool preProcessFO(crewMember memberData[]) {
// 	int crewNum = isDataA? crewNumA : crewNumB;
// 	sort(memberData + 1, memberData + crewNum, FO_Cmp);
// 	if(memberData[1].level != 3 || memberData[1].isUsed)	//	如果纯副机长都用光了,或不可用，返回 null
// 		return false;
// 	return true;
// }


void splitString(int x){
	int index = 0;
	int number = 0;
	string temString[10];
	for(int i = 0; i < preData[x].length(); i++){
		if(preData[x][i] == ','){
			temString[number++] = preData[x].substr(index, i - index);
			index = i + 1;
		}
	}temString[number] = preData[x].substr(index, preData[x].length() - index);
	/*
	cout << "x = " << x << " ";
	for(int i = 0; i < 8;i++){
		cout << temString[i] << " ";
	}cout << endl;
	*/
	data[x].fltNum = temString[0];
	data[x].startDate = temString[1];
	data[x].startTime = temString[2];
	data[x].startPlace = temString[3];
	data[x].endDate = temString[4];
	data[x].endTime = temString[5];
	data[x].endPlace = temString[6];
	data[x].C = int(temString[7][1] - '0');
	data[x].F = int(temString[7][3] - '0');
	data[x].isUsed = false;
	
}

void calUniquePlace(){
	set<string>s;
	for(int i = 1; i < N; i++){
		s.insert(data[i].startPlace);
		s.insert(data[i].endPlace);
	}
	map<string,int>m;
	map<int,string>oppositeM; 
	int number = 0;
	for(set<string>::iterator it = s.begin(); it != s.end(); it++){
		if(number == 0){
			number++;
			continue;
		} 
		oppositeM[number] = *it;
		m[*it] = number++;
	}cout << endl; 
	vector< vector<int> >v(40, vector<int>(40, 0));
	for(int i = 1; i < N; i++){
		//cout << m[data[i].startPlace] << " " << m[data[i].endPlace] << endl; 
		v[m[data[i].startPlace]][m[data[i].endPlace]] = 1;
	}
	
	for(int i = 0; i < v.size(); i++){
		//cout << i << oppositeM[i] << ":"; 
		//cout << i  << ":"; 
		for(int j = 0; j < v[i].size(); j++){
			if(v[i][j] == 1){
				//cout << j << oppositeM[j] << " "; 
				//cout << j << " "; 
//				 cout << "{ source: "<< i-1 << ", target: " << j-1 << " }," << endl; 
			}
		}//cout << endl;
	}
	 
}

void initMember() {
	ifstream myfile(isDataA? crewFileA : crewFileB);
    if( !myfile.is_open()){
    	cout << "Can not open" << endl;
		return;
    }
	int crewNum = isDataA? crewNumA : crewNumB;
	for(int i = 0; i < crewNum; i++){
		myfile >> preDataCrew[i];
	}
	for(int i = 1; i < crewNum; i++){
		vector<string> value = split(preDataCrew[i], ',');
		memberData[i].empNo = value[0];
		memberData[i].captain = value[1] == "Y" ? 1 : 0;
		memberData[i].firstOfficer = value[2] == "Y" ? 1 : 0;
		memberData[i].deadhead = value[3] == "Y" ? 1 : 0;
		memberData[i].base = memberData[i].nowBase = value[4];
		memberData[i].dutyCostPerHour = atoi(value[5].c_str());
		memberData[i].paringCostPerHour = atoi(value[6].c_str());
		memberData[i].isUsed = false;
		memberData[i].haveNight = false;
		if(memberData[i].captain && memberData[i].firstOfficer)	//	初始化机组人员的 level
			memberData[i].level = 1;
		else if(memberData[i].captain)
			memberData[i].level = 2;
		else
			memberData[i].level = 3;
		//cout <<"i:" << i << " " <<memberData[i].empNo << " " << memberData[i].captain << endl; 
	}
	
	for(int i = 1; i < 500; i++){
		//record[i] = (crewMember*)malloc(sizeof(crewMember) * 500);
	}
}
 


void init(){
	FILE *fp;
	int i,j ;
    ifstream myfile(isDataA? flightFileA : flightFileB);
    if( !myfile.is_open()){
    	cout << "Can not open" << endl;
		return;
    }
	int flightNum = isDataA? flightNumA : flightNumB;
	for(int i = 0; i < flightNum; i++){
		myfile >> preData[i];
	}
	
	for(int i = 1; i < flightNum; i++){
		splitString(i);
	}
	calUniquePlace();
	
	
}

void printProblemAns(){
	int canNotjustPlane = 0; 
	for(int i = 1; i < flightNumA; i++){
		if(data[i].isUsed == false){
			cout << "i:" << i << endl;
			canNotjustPlane++;
		}
	}
	//cout << "不满足机组配置航班数：" << canNotjustPlane << " 满足机组配置航班数：" << flightNumA - canNotjustPlane << " 机组人员总体乘机次数：" << sumMemberTakePlane * 2 << " 替补资格使用次数:0" << endl; 
}

inline void setMemberUsed(crewMember & member, fightData & flight, int & canUseMemberNum, int & couldUseMemberNum, int & sumMemberTakePlane) {
	member.isUsed = true;
	member.endTime = flight.endTime;
	member.endDate = flight.endDate;
	member.nowBase = flight.endPlace;
	flight.isUsed = true;
	canUseMemberNum--;
	couldUseMemberNum = canUseMemberNum;
	sumMemberTakePlane++;
}

void setRecord(int x){
	int crewNum = isDataA? crewNumA : crewNumB;
	if(x != 0 && x != 1200  && x % 1200 == 0){
		alreadyUseNum++;
		for(int i = 1;i < 1200; i++){
			//record[i] = record[i + 1200];
			memcpy(record[i], record[i + 1200], sizeof(crewMember) * 500);
		}
	}
	for(int i = 1; i <= crewNum; i++){
		record[x - 1200 * alreadyUseNum][i] = memberData[i];
	}
}

void calFirst(){
	
	int flightNum = isDataA? flightNumA : flightNumB;
	int crewNum = isDataA? crewNumA : crewNumB;
	int startTime = 11;
	int canUseMemberNum = 10;
	sort(data + 1, data + flightNum, lessThen);
	int lastUpdateNum = 0; 
	int oneDayNumber = 0;
	int lastOneDayNumber = 0;
	for(int i = 1 ; i < flightNum; i++){
		oneDayNumber++;
		if(atoi(split(data[i].startDate, '/')[1].c_str()) == startTime){
			int idxOfCA = -1, idxOfFO = -1;
			pair<int, int> group = chooseCrew(memberData);
			if(group.first < 0 || group.second < 0) {	//	假如现在就能判断当前航班凑不满人，就不能飞，跳过看下一班
				continue;
			} else {	//	有可能能凑满人，看看具体选谁
				//	CA 和 FO 分别从刚刚找到的可能合法的下标处开始找
				int findCA = false, findFO = false;
				int posFO = group.first, posCA = group.second;
				// for(; posFO < crewNum && posCA < group.first;) {
				// 	//	当前飞行员没被用，在当前机场，休息时间够了
				// 	if(memberData[posFO].isUsed == false && memberData[posFO].base == data[i].startPlace && subtraction_end(data[i].startDate, data[i].startTime, memberData[posFO].endDate, memberData[posFO].endTime) >= 40) {
				// 		findFO = true;
				// 	} else 
				// 		++posFO;
				// 	if(memberData[posCA].isUsed == false && memberData[posCA].base == data[i].startPlace && subtraction_end(data[i].startDate, data[i].startTime, memberData[posCA].endDate, memberData[posCA].endTime) >= 40) {
				// 		findCA = true;
				// 	} else 
				// 		++posCA;
				// 	if(findCA && findFO)
				// 		break;
				// }
				while(posFO < crewNum) {
					if(memberData[posFO].isUsed == false && memberData[posFO].nowBase == data[i].startPlace && subtraction_end(data[i].startDate, data[i].startTime, memberData[posFO].endDate, memberData[posFO].endTime) >= 40) {
						findFO = true;
						break;
					}  
					++posFO;
				}
				while(posCA < group.first) {
					if(memberData[posCA].isUsed == false && memberData[posCA].nowBase == data[i].startPlace && subtraction_end(data[i].startDate, data[i].startTime, memberData[posCA].endDate, memberData[posCA].endTime) >= 40) {
						findCA = true;
						break;
					} 
					++posCA;
				}
				if(!findCA || !findFO) {	//	遍历后发现不能飞，跳过看下一班
					continue;
				} else {	//	发现可以飞，那么更新飞行员和机场状态
					setMemberUsed(memberData[posFO], data[i], canUseMemberNum, couldUseMemberNum[i], sumMemberTakePlane);	//	couldUseMemberNum 不太懂
					setMemberUsed(memberData[posCA], data[i], canUseMemberNum, couldUseMemberNum[i], sumMemberTakePlane);
				}
			}

		}
		if(i == flightNum - 1){
			continue; 
		}
		if(atoi(split(data[i + 1].startDate, '/')[1].c_str()) == startTime + 1){	//	当日航班结束，重置状态
			for(int j = 1; j <= crewNum; j++){
				// if(memberData[j].nowBase != memberData[j].base)
				// 	memberData[j].haveNight = true;
				if(memberData[j].haveNight == true){
					continue;
				}
				memberData[j].isUsed = false;
				memberData[j].endDate = "8/0/2021";
				memberData[j].endTime = "0:0";
			}
		}
	
		for(int j = 1; j < crewNum; j++){
			if(memberData[j].isUsed == true){
				//cout << memberData[j].endDate << " " <<  memberData[j].endTime << " " << data[i].startDate << " " << data[i].startTime << endl;
				if(lessThen_end(memberData[j].endDate, memberData[j].endTime, data[i + 1].startDate, data[i + 1].startTime)){
					memberData[j].isUsed = false;
				}
			}
		}
		setRecord(i);

		if(atoi(split(data[i + 1].startDate, '/')[1].c_str()) == startTime + 1){
			if(lastUpdateNum != 0){
				//cout << "i:" << i << " " << "lastUpdateNum:" << lastUpdateNum << " lastOneDayNumber:" << lastOneDayNumber << endl;
				// lastOneDayNumber: 上周期同一天的天数
				//oneDayNumber:当前周期同一天的天数
				//lastUpdateNum:进入新一天的下标

				//计算当天可带人
				for(int j = i; j > lastUpdateNum; j--){
					if(data[j].isUsed == false){ //如果存在没安排到的航班
						for(int k = j - 1; k > lastUpdateNum; k--){
							if(data[j].startPlace == data[k].endPlace){ //如果可以过一天等待带人过去，接下判断下带人会不会导致当天人不够
								crewMember *temMemberData = record[k];
								sort(temMemberData + 1, temMemberData + crewNum, choose_Cmp);
								pair<int, int> group = chooseCrew(temMemberData);
								if(group.first < 0 || group.second < 0) {	//	假如现在就能判断当前航班凑不满人，就不能飞，跳过看下一班
									continue;
								} else {	//	有可能能凑满人，看看具体选谁
									//	CA 和 FO 分别从刚刚找到的可能合法的下标处开始找
									int findCA = false, findFO = false;
									int posFO = group.first + j - k + 1, posCA = group.second + j - k + 1;
									//cout << "i:" << i << " CCC：posFO" << posFO << " posCA" << posCA << endl;
									while(posFO < crewNum) {
										if(temMemberData[posFO].isUsed == false && temMemberData[posFO].nowBase == data[k].startPlace && subtraction_end(data[j].startDate, data[j].startTime, temMemberData[posFO].endDate, temMemberData[posFO].endTime) >= 40) {
											findFO = true;
											break;
										} else{
											cout << temMemberData[posFO].nowBase << " " << subtraction_end(data[j].startDate, data[j].startTime, temMemberData[posFO].endDate, temMemberData[posFO].endTime) << endl;
										}
										++posFO;
									}
									while(posCA < group.first) {
										if(temMemberData[posCA].isUsed == false  && temMemberData[posCA].nowBase == data[k].startPlace && subtraction_end(data[j].startDate, data[j].startTime, temMemberData[posCA].endDate, temMemberData[posCA].endTime) >= 40) {
											findCA = true;
											break;
										} 
										++posCA;
									}
									//cout << "posFO" << posFO << " posCA" << posCA << endl;
									if(!findCA || !findFO) {	//	遍历后发现不能飞，跳过看下一班
										//puts("不能飞");
										continue;
									} else {	//	发现可以飞，那么更新飞行员和机场状态
										setMemberUsed(temMemberData[posFO], data[i], canUseMemberNum, couldUseMemberNum[i], sumMemberTakePlane);	//	couldUseMemberNum 不太懂
										setMemberUsed(temMemberData[posCA], data[i], canUseMemberNum, couldUseMemberNum[i], sumMemberTakePlane);
										data[j].isUsed = true;
										sumMemberTakePlane += 2;
									}
								}
							}
						}
					}
				}
				//计算上一天是否要带人
				for(int j = lastUpdateNum; j <= i; j++){
					if(data[j].isUsed == false){ //如果存在没安排到的航班
						for(int k = lastUpdateNum - 1; k >= lastUpdateNum - lastOneDayNumber; k--){
							if(data[j].startPlace == data[k].endPlace){ //如果可以过一天等待带人过去，接下判断下带人会不会导致当天人不够
								crewMember *temMemberData = record[k];
								//维护一个map记录中间过程使用过的员工 
								map<string, int>m;
								for(int si = k; si < j; si++){
									crewMember *stemMemberData = record[si];
									for(int sj = 1; sj < 500; sj++){
										if(stemMemberData[sj].isUsed == 1){
											m[stemMemberData[sj].empNo] = 1;
										}
										
									}
								}
								for(auto x: m){
									cout << x.first << " " << x.second << endl;
								}
								sort(temMemberData + 1, temMemberData + crewNum, choose_Cmp);
								pair<int, int> group = chooseCrew(temMemberData);
								if(group.first < 0 || group.second < 0) {	//	假如现在就能判断当前航班凑不满人，就不能飞，跳过看下一班
									continue;
								} else {	//	有可能能凑满人，看看具体选谁
									//	CA 和 FO 分别从刚刚找到的可能合法的下标处开始找
									int findCA = false, findFO = false;
									int posFO = group.first, posCA = group.second;
									cout << "i:" << i << " posCA：" << posCA << " posFO：" << posFO << endl;
									while(posFO < crewNum) {
										if(temMemberData[posFO].isUsed == false && temMemberData[posFO].nowBase == data[k].startPlace && subtraction_end(data[i].startDate, data[i].startTime, memberData[posFO].endDate, memberData[posFO].endTime) >= 40
											&& m[temMemberData[posFO].empNo] == 0 ) {
											findFO = true;
											break;
										} 
										cout << "m: " << m[temMemberData[posFO].empNo] << " empNo: " << temMemberData[posFO].empNo << endl;
										++posFO;
									}
									while(posCA < group.first) {
										if(temMemberData[posCA].isUsed == false && temMemberData[posCA].nowBase == data[k].startPlace && subtraction_end(data[i].startDate, data[i].startTime, memberData[posCA].endDate, memberData[posCA].endTime) >= 40
											&& m[temMemberData[posCA].empNo] == 0) {
											findCA = true;
											break;
										} 
										++posCA;
									}
									cout << "posFO" << posFO << " posCA" << posCA << endl;
									if(!findCA || !findFO) {	//	遍历后发现不能飞，跳过看下一班
										//puts("不能飞");
										continue;
									} else {	//	发现可以飞，那么更新飞行员和机场状态
										setMemberUsed(temMemberData[posFO], data[i], canUseMemberNum, couldUseMemberNum[i], sumMemberTakePlane);	//	couldUseMemberNum 不太懂
										setMemberUsed(temMemberData[posCA], data[i], canUseMemberNum, couldUseMemberNum[i], sumMemberTakePlane);
										data[j].isUsed = true;
										sumMemberTakePlane += 2;

									}
								}
									
							}
						}
					}
				}
			}
			lastUpdateNum = i + 1;
			startTime++;
			lastOneDayNumber = oneDayNumber;
			oneDayNumber = 0;

		}
	}
	
	for(int i = 1; i < flightNum; ++i) {
		cout <<"i = " << i << " "<< data[i].startDate << " " << data[i].startTime << " " << data[i].startPlace <<
		" " <<  data[i].endDate << " " << data[i].endTime << " " << data[i].endPlace << " " <<  data[i].C << " " << data[i].F << " " << data[i].isUsed << endl; 
	}
	printProblemAns();
	
}



int main(){
	init();
	initMember();
	calFirst();
	// int crewNum = isDataA? crewNumA : crewNumB;
	// sort(memberData + 1, memberData + crewNum, FO_Cmp);
	// for(int j = 1; j < crewNum; ++j)
	// 	cout << memberData[j].empNo << " " <<  memberData[j].level << " " << endl;
	// crewMember * a = getCA(memberData);
	// puts("------------");
	// cout << a->empNo << " " <<  a->level << " " << endl;
	// a = getFO(memberData);
	// puts("------------");
	// cout << a->empNo << " " <<  a->level << " " << endl;
	return 0;
} 




