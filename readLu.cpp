#pragma GCC optimize(2)
#pragma GCC optimize(3)
#include <bits/stdc++.h>
#include "class.h"

using namespace std;

const int N = 14000; 
const int MinCT = 40, MaxBlk = 600, MaxDP = 720;
// const int NA = 207; 
string preData[N];
string preDataCrew[N];
const string crewFileA = "�����Ű�Data A-Crew.csv", crewFileB = "�����Ű�Data B-Crew.csv";
const string flightFileA = "�����Ű�Data A-Flight.csv", flightFileB = "�����Ű�Data B-Flight.csv";
const int flightNumA = 207, flightNumB = 13955;
const int crewNumA = 22, crewNumB = 466;	//	������ͷ��һ�У�ʵ�����ݴ� 1 ��ʼ
bool isDataA = 0;

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
	if(a.level < b.level)	//	����ȡ 1��2
		return true;
	else if(a.level == b.level) {
		// int flag = 2;
		// try {
		// 	flag = !lessThen_end(a.endDate, a.endTime, b.endDate, b.endTime);
		// }
		// catch(const std::exception& e)
		// {
		// 	std::cerr << e.what() << '\n';
		// }
		
		return !lessThen_end(a.endDate, a.endTime, b.endDate, b.endTime);	//	����ȡ����ʱ����������ڽ����ģ�����ס�Ƶ�ʱ��
	} else
		return false;
}

bool orderByDutyMin(const crewMember & a, const crewMember & b) {
	return a.dutyMin < b.dutyMin;
}

// bool FO_Cmp(const crewMember & a, const crewMember & b) {
// 	if(a.isUsed != b.isUsed)
// 		return a.isUsed == false? true : false;
// 	if(a.level > b.level)	//	����ȡ 3
// 		return true;
// 	else if(a.level == b.level) {
// 		return !lessThen_end(a.endDate, a.endTime, b.endDate, b.endTime);
// 	} else
// 		return false;
// }

pair<int, int> chooseCrew(crewMember memberData[], int flag = 1) {	//	�������Ա������ {FO��ʼ�±꣬CA��ʼ�±�}���Ҳ����Ϸ���Ϊ -1
	int crewNum = isDataA? crewNumA : crewNumB;
	sort(memberData + 1, memberData + crewNum, choose_Cmp);
	int pos = 1;
	while(memberData[pos].level != 3)
		pos++;
	if(pos >= crewNum)
		return make_pair(-1, -1);
	if(flag == 2) {
		puts("++++++++++++++++++++++++++++++++");
	}
	sort(memberData + 1, memberData + pos, orderByDutyMin);
	if(flag == 2) {
		puts("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
		// cout << "pos = " << pos << " member = " << memberData[pos].level << endl;
		for(int k = 1; k < crewNum; ++k)
			cout << memberData[k].level;
		cout << endl;
 	}
	sort(memberData + pos, memberData + crewNum, orderByDutyMin);
	if(flag == 2) {
		puts("--------------------------------");
	}
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
// 	if(memberData[1].level != 3 || memberData[1].isUsed)	//	��������������ù���,�򲻿��ã����� null
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
		if(memberData[i].captain && memberData[i].firstOfficer)	//	��ʼ��������Ա�� level
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
	//cout << "������������ú�������" << canNotjustPlane << " ����������ú�������" << flightNumA - canNotjustPlane << " ������Ա����˻�������" << sumMemberTakePlane * 2 << " �油�ʸ�ʹ�ô���:0" << endl; 
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
	member.dutyMin += subtraction_end(flight.endDate, flight.endTime, flight.startDate, flight.startTime);
}

void setRecord(int x){
	if(x > 11999)
		return;
	int crewNum = isDataA? crewNumA : crewNumB;
	if(x != 0 && x != 1200  && x % 1200 == 0){
		alreadyUseNum++;
		for(int i = 1;i < 1200; i++){
			//record[i] = record[i + 1200];
			// memcpy(record[i], record[i + 1200], sizeof(crewMember) * 500);
			for(int k = 0; k < 500; ++k) {
				record[i][k] = record[i + 1200][k];
			}
		}
	}
	for(int i = 1; i < crewNum; i++){
		record[x - 1200 * alreadyUseNum][i] = memberData[i];
	}
}

void calFirst(){
	
	int flightNum = isDataA? flightNumA : flightNumB;
	int crewNum = isDataA? crewNumA : crewNumB;
	int startTime = 1;
	int canUseMemberNum = 10;
	sort(data + 1, data + flightNum, lessThen);
	int lastUpdateNum = 0; 
	int oneDayNumber = 0;
	int lastOneDayNumber = 0;
	for(int i = 1 ; i < flightNum; i++){
		cout << "i = " << i << endl;
		oneDayNumber++;
		if(atoi(split(data[i].startDate, '/')[1].c_str()) == startTime){
			int idxOfCA = -1, idxOfFO = -1;
			// if(i > 11990)
			// 	puts("aaaaaaaaaaaaaaaaaaaaaaaaaaa");
			pair<int, int> group = chooseCrew(memberData);
			if(group.first < 0 || group.second < 0) {	//	�������ھ����жϵ�ǰ����ղ����ˣ��Ͳ��ܷɣ���������һ��
				continue;
			} else {	//	�п����ܴ����ˣ���������ѡ˭
				//	CA �� FO �ֱ�Ӹո��ҵ��Ŀ��ܺϷ����±괦��ʼ��
				int findCA = false, findFO = false;
				int posFO = group.first, posCA = group.second;
				// for(; posFO < crewNum && posCA < group.first;) {
				// 	//	��ǰ����Աû���ã��ڵ�ǰ��������Ϣʱ�乻��
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
				// if(i > 11990)
				// 	puts("bbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
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
				if(!findCA || !findFO) {	//	�������ֲ��ܷɣ���������һ��
					continue;
				} else {	//	���ֿ��Էɣ���ô���·���Ա�ͻ���״̬
					setMemberUsed(memberData[posFO], data[i], canUseMemberNum, couldUseMemberNum[i], sumMemberTakePlane);	//	couldUseMemberNum ��̫��
					setMemberUsed(memberData[posCA], data[i], canUseMemberNum, couldUseMemberNum[i], sumMemberTakePlane);
				}
			}
			// if(i > 11990)
			// 	puts("ccccccccccccccccccccccccccccccccc");
		}
		if(i == flightNum - 1){
			continue; 
		}
		if(atoi(split(data[i + 1].startDate, '/')[1].c_str()) == startTime + 1){	//	���պ������������״̬
			for(int j = 1; j < crewNum; j++){
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
		// if(i > 11990)
		// 	puts("ddddddddddddddddddddddddd");
		for(int j = 1; j < crewNum; j++){
			// if(i > 11999)
			// 	cout << memberData[j].level << endl;
			if(memberData[j].isUsed == true){
				//cout << memberData[j].endDate << " " <<  memberData[j].endTime << " " << data[i].startDate << " " << data[i].startTime << endl;
				if(lessThen_end(memberData[j].endDate, memberData[j].endTime, data[i + 1].startDate, data[i + 1].startTime)){
					memberData[j].isUsed = false;
				}
			}
		}
		// if(i > 11990)
		// 	puts("fffffffffffffffffffffffff");
		setRecord(i);

		if(i > 11990)
			puts("eeeeeeeeeeeeeeeeeeeeeeeeeeeee");
		// puts("11111111111111111111111111");
		// cout << "i = " << i << " " << atoi(split(data[i + 1].startDate, '/')[1].c_str()) << endl;
		if(atoi(split(data[i + 1].startDate, '/')[1].c_str()) == startTime + 1){
			if(lastUpdateNum != 0){
				//cout << "i:" << i << " " << "lastUpdateNum:" << lastUpdateNum << " lastOneDayNumber:" << lastOneDayNumber << endl;
				// lastOneDayNumber: ������ͬһ�������
				//oneDayNumber:��ǰ����ͬһ�������
				//lastUpdateNum:������һ����±�
				if(i > 13000)
					puts("222222222222222222222222");
				//���㵱��ɴ���
				for(int j = i; j > lastUpdateNum; j--){
					if(data[j].isUsed == false){ //�������û���ŵ��ĺ���
						for(int k = j - 1; k > lastUpdateNum; k--){
							if(data[j].startPlace == data[k].endPlace){ //������Թ�һ��ȴ����˹�ȥ�������ж��´��˻᲻�ᵼ�µ����˲���
								crewMember *temMemberData = record[k - 1200 * alreadyUseNum];
								if(i > 13000)
									puts("333333333333333333");
								sort(temMemberData + 1, temMemberData + crewNum, choose_Cmp);//	14:34 ���ֳ�����
								if(i > 13000)
									puts("44444444444444444444");
								int ttt = 1;
								if(i == 1795)
									ttt = 2;
								pair<int, int> group = chooseCrew(temMemberData, ttt);
								// puts("55555555555555555555555");
								if(group.first < 0 || group.second < 0) {	//	�������ھ����жϵ�ǰ����ղ����ˣ��Ͳ��ܷɣ���������һ��
									continue;
								} else {	//	�п����ܴ����ˣ���������ѡ˭
									//	CA �� FO �ֱ�Ӹո��ҵ��Ŀ��ܺϷ����±괦��ʼ��
									int findCA = false, findFO = false;
									int posFO = group.first + j - k + 1, posCA = group.second + j - k + 1;
									//cout << "i:" << i << " CCC��posFO" << posFO << " posCA" << posCA << endl;
									while(posFO < crewNum) {
										if(temMemberData[posFO].isUsed == false && temMemberData[posFO].nowBase == data[k].startPlace && subtraction_end(data[j].startDate, data[j].startTime, temMemberData[posFO].endDate, temMemberData[posFO].endTime) >= 40) {
											findFO = true;
											break;
										} else{
											// cout << temMemberData[posFO].nowBase << " " << subtraction_end(data[j].startDate, data[j].startTime, temMemberData[posFO].endDate, temMemberData[posFO].endTime) << endl;
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
									if(!findCA || !findFO) {	//	�������ֲ��ܷɣ���������һ��
										//puts("���ܷ�");
										continue;
									} else {	//	���ֿ��Էɣ���ô���·���Ա�ͻ���״̬
										setMemberUsed(temMemberData[posFO], data[i], canUseMemberNum, couldUseMemberNum[i], sumMemberTakePlane);	//	couldUseMemberNum ��̫��
										setMemberUsed(temMemberData[posCA], data[i], canUseMemberNum, couldUseMemberNum[i], sumMemberTakePlane);
										data[j].isUsed = true;
										sumMemberTakePlane += 2;
									}
								}
							}
						}
					}
				}
				//������һ���Ƿ�Ҫ����
				for(int j = lastUpdateNum; j <= i; j++){
					if(data[j].isUsed == false){ //�������û���ŵ��ĺ���
						for(int k = lastUpdateNum - 1; k >= lastUpdateNum - lastOneDayNumber; k--){
							// puts("6666666666666666666666666666");
							if(data[j].startPlace == data[k].endPlace){ //������Թ�һ��ȴ����˹�ȥ�������ж��´��˻᲻�ᵼ�µ����˲���
								crewMember *temMemberData = record[k - 1200 * alreadyUseNum];
								//ά��һ��map��¼�м����ʹ�ù���Ա�� 
								map<string, int>m;
								// puts("77777777777777777777777777");
								for(int si = k; si < j; si++){
									crewMember *stemMemberData = record[si - alreadyUseNum * 1200];
									for(int sj = 1; sj < 500; sj++){
										if(stemMemberData[sj].isUsed == 1){
											m[stemMemberData[sj].empNo] = 1;
										}
										
									}
								}
								// puts("88888888888888888888888888");
								// for(auto x: m){
								// 	cout << x.first << " " << x.second << endl;
								// }
								sort(temMemberData + 1, temMemberData + crewNum, choose_Cmp);
								pair<int, int> group = chooseCrew(temMemberData);
								if(group.first < 0 || group.second < 0) {	//	�������ھ����жϵ�ǰ����ղ����ˣ��Ͳ��ܷɣ���������һ��
									continue;
								} else {	//	�п����ܴ����ˣ���������ѡ˭
									//	CA �� FO �ֱ�Ӹո��ҵ��Ŀ��ܺϷ����±괦��ʼ��
									int findCA = false, findFO = false;
									int posFO = group.first, posCA = group.second;
									// cout << "i:" << i << " posCA��" << posCA << " posFO��" << posFO << endl;
									while(posFO < crewNum) {
										if(temMemberData[posFO].isUsed == false && temMemberData[posFO].nowBase == data[k].startPlace && subtraction_end(data[i].startDate, data[i].startTime, memberData[posFO].endDate, memberData[posFO].endTime) >= 40
											&& m[temMemberData[posFO].empNo] == 0 ) {
											findFO = true;
											break;
										} 
										// cout << "m: " << m[temMemberData[posFO].empNo] << " empNo: " << temMemberData[posFO].empNo << endl;
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
									// puts("88888888888888888888888888");
									// cout << "posFO" << posFO << " posCA" << posCA << endl;
									if(!findCA || !findFO) {	//	�������ֲ��ܷɣ���������һ��
										//puts("���ܷ�");
										continue;
									} else {	//	���ֿ��Էɣ���ô���·���Ա�ͻ���״̬
										setMemberUsed(temMemberData[posFO], data[i], canUseMemberNum, couldUseMemberNum[i], sumMemberTakePlane);	//	couldUseMemberNum ��̫��
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
	ios::sync_with_stdio(false);
	init();
	initMember();
	calFirst();
	int crewNum = isDataA? crewNumA : crewNumB;
	// sort(memberData + 1, memberData + crewNum, FO_Cmp);
	for(int j = 1; j < crewNum; ++j)
		cout << memberData[j].dutyMin << " " <<  memberData[j].level << " " << endl;
	// crewMember * a = getCA(memberData);
	// puts("------------");
	// cout << a->empNo << " " <<  a->level << " " << endl;
	// a = getFO(memberData);
	// puts("------------");
	// cout << a->empNo << " " <<  a->level << " " << endl;
	return 0;
} 




