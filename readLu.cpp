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

crewMember memberData[N + 5];

fightData data[N + 5]; 

bool CO_Cmp(const crewMember & a, const crewMember & b) {
	if(a.isUsed != b.isUsed)
		return a.isUsed == false? true : false;
	return a.level < b.level;	//	优先取 1，2
}

bool FO_Cmp(const crewMember & a, const crewMember & b) {
	if(a.isUsed != b.isUsed)
		return a.isUsed == false? true : false;
	return a.level > b.level;	//	优先取 3
}

crewMember * getCO(crewMember memberData[]) {	//	注意：返回的是对象的指针，get到后可以直接修改的
	int crewNum = isDataA? crewNumA : crewNumB;
	sort(memberData + 1, memberData + crewNum, CO_Cmp);
	if(memberData[0].level == 3)	//	如果只剩下纯副机长，返回 null
		return nullptr;
	return &memberData[0];
}

crewMember * getFO(crewMember memberData[]) {
	int crewNum = isDataA? crewNumA : crewNumB;
	sort(memberData + 1, memberData + crewNum, FO_Cmp);
	if(memberData[0].level != 3)	//	如果纯副机长都用光了，返回 null
		return nullptr;
	return &memberData[0];
}


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
		memberData[i].base = value[4];
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

void calFirst(){
	int flightNum = isDataA? flightNumA : flightNumB;
	int startTime = 11;
	int canUseMemberNum = 10; 
	sort(data + 1, data + flightNum, lessThen);
	int lastUpdateNum = 0; 
	int oneDayNumber = 0;
	int lastOneDayNumber = 0;
	for(int i = 1 ; i < flightNum; i++){
		oneDayNumber++;
		if(atoi(split(data[i].startDate, '/')[1].c_str()) == startTime){
			bool temGiveMember = false;
			for(int j = 1; j <= 10; j++){
				//cout << "i = " << i << "ans: " << subtraction_end(data[i].startDate, data[i].startTime, memberData[j].endDate, memberData[j].endTime) << " " << memberData[i].base << endl;
				if(memberData[j].isUsed == false && data[i].startPlace == memberData[j].base && (subtraction_end(data[i].startDate, data[i].startTime, memberData[j].endDate, memberData[j].endTime) >= 40)) {
					memberData[j].isUsed = true;
					memberData[j].endTime = data[i].endTime;
					memberData[j].endDate = data[i].endDate; 
					memberData[j].base = data[i].endPlace;
					data[i].isUsed = true;
					canUseMemberNum--;
					couldUseMemberNum[i] = canUseMemberNum;
					sumMemberTakePlane++;
					temGiveMember = true;
					//cout << "i2:"<< i << "base:" << memberData[j].base;
					break;
				}
			}
			if(temGiveMember == false){
				couldUseMemberNum[i] = couldUseMemberNum[i - 1];
			}
		}
		//cout << "i*"<< endl;
		if(i == flightNum - 1){
			continue; 
		}
		if(atoi(split(data[i + 1].startDate, '/')[1].c_str()) == startTime + 1){
			for(int j = 1; j <= 10; j++){
				if(memberData[j].haveNight == true){
					continue;
				}
				memberData[j].isUsed = false;
				memberData[j].endDate = "8/0/2021";
				memberData[j].endTime = "0:0";
			}
		}
	
		for(int j = 1; j <= 10; j++){
			if(memberData[j].isUsed == true){
				//cout << "i5" << i << " " << lessThen_end(memberData[j].endDate, memberData[j].endTime, data[i].startDate, data[i].startTime) << endl;
				//cout << memberData[j].endDate << " " <<  memberData[j].endTime << " " << data[i].startDate << " " << data[i].startTime << endl;
				if(lessThen_end(memberData[j].endDate, memberData[j].endTime, data[i + 1].startDate, data[i + 1].startTime)){
					memberData[j].isUsed = false;
					canUseMemberNum++;
					couldUseMemberNum[i] = canUseMemberNum;
				}
			}
		}
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
								bool couldHaveNight = true;
								for(int z = k; z < j; z++){//这里要查询过去人员表使用情况
									if(couldUseMemberNum[z] <= 0){
										couldHaveNight = false;
										cout << "k:" << k << " 当天第" << z << "天的人数不支持  " ;
										cout << couldUseMemberNum[z] << endl;
									}
								}
								if(couldHaveNight == true){
									for(int z = k; z < j; z++){
										couldUseMemberNum[z]--;
									}
									cout << "当天第" << j  << "条没排"<< " ，通过第" << k << "条带人完成" << endl;
									data[j].isUsed = true;
									sumMemberTakePlane++;
								}else{
									cout << "当天第" << j  << "条没排"<< "并且当天不支持排" << endl;
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
								bool couldHaveNight = true;
								for(int z = k; z < j; z++){//这里要查询过去人员表使用情况
									if(couldUseMemberNum[z] <= 0){
										couldHaveNight = false;
										cout << "第" << z << "天的人数不支持" << endl;
									}
								}
								if(couldHaveNight == true){
									for(int z = k; z < j; z++){
										couldUseMemberNum[z]--;
									}
									cout << "第" << j  << "条没排"<< " ，通过第" << k << "条带人完成" << endl;
									data[j].isUsed = true;
									sumMemberTakePlane++;
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
//	int crewNum = isDataA? crewNumA : crewNumB;
//	sort(memberData + 1, memberData + crewNum, FO_Cmp);
//	for(int j = 1; j < crewNum; ++j)
//		cout << memberData[j].empNo << " " <<  memberData[j].level << " " << endl;
	return 0;
} 




