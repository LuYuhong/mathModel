#include <bits/stdc++.h>

using namespace std;

const int N = 14000; 
const int MinCT = 40, MaxBlk = 600, MaxDP = 720;
const int NA = 207; 
string preData[N];

struct crewMember {
	string empNo;
	bool captain;
	bool firstOfficer;
	bool deadhead;
	string base;
	int dutyCostPerHour;
	int paringCostPerHour;
	bool isUsed;
	string endDate = "8/0/2021";
	string endTime = "0:0";

	
	void printInfo() {
		//cout << this->EmpNo << ' ' << this->isUsed << " " << this->endTime << endl;
	}
} memberData[N + 5];

struct fightData{
	string fltNum;
	string startDate;
	string startTime;
	string startPlace;
	string endDate;
	string endTime;
	string endPlace;
	int C;
	int F;
	bool isUsed;
	void printInfo() {
		cout << this->fltNum << ' ' << this->isUsed << " " << this->endTime << endl;
	}
}data[N + 5]; 

vector<string> split(string s, char c) {
	vector<string> ans;
	string now = "";
	for(char x : s) {
		if(x == c) {
			ans.push_back(now);
			now = "";
			continue;
		}
		now += x;
	}
	if(now.length() > 0)
		ans.push_back(now);
	return ans;
}

vector<int> getTime(string date, string time) {
	int day = atoi(split(date, '/')[1].c_str());
	int h = atoi(split(time, ':')[0].c_str());
	int m = atoi(split(time, ':')[1].c_str());
	vector<int> Time;
	Time.push_back(day);
	Time.push_back(h);
	Time.push_back(m);
	return Time;
}

bool lessThen(fightData & a, fightData & b) {	//	a < b 返回 true 
	//cout << "1" << endl; 
	string date1 = a.startDate, date2 = b.startDate;
	string time1 = a.startTime, time2 = b.startTime;
	vector<int> Time1 = getTime(date1, time1), Time2 = getTime(date2, time2);
	int day1 = Time1[0], day2 = Time2[0];
	int h1 = Time1[1], h2 = Time2[1];
	int m1 = Time1[2], m2 = Time2[2];
//	int day1 = atoi(split(date1, '/')[1].c_str()), day2 = atoi(split(date2, '/')[1].c_str());
//	int h1 = atoi(split(time1, ':')[0].c_str()), h2 = atoi(split(time2, ':')[0].c_str());
//	int m1 = atoi(split(time1, ':')[1].c_str()), m2 = atoi(split(time1, ':')[1].c_str());
//	cout << day1 << " " << day2 << endl;
	//cout << h1 << " " << m1 << endl;
	if(day1 < day2) {
		return true;
	} else if(day1 == day2) {
		if(h1 < h2)
			return true;
		else if(h1 == h2)
			return m1 < m2;
		return false;
	} else
		return false;
}
bool lessThen_end(string date1, string time1, string date2, string time2) {	//	a < b 返回 true 
	//cout << "1" << endl; 
//	string date1 = a.startDate, date2 = b.startDate;
//	string time1 = a.startTime, time2 = b.startTime;
	vector<int> Time1 = getTime(date1, time1), Time2 = getTime(date2, time2);
	int day1 = Time1[0], day2 = Time2[0];
	int h1 = Time1[1], h2 = Time2[1];
	int m1 = Time1[2], m2 = Time2[2];
//	int day1 = atoi(split(date1, '/')[1].c_str()), day2 = atoi(split(date2, '/')[1].c_str());
//	int h1 = atoi(split(time1, ':')[0].c_str()), h2 = atoi(split(time2, ':')[0].c_str());
//	int m1 = atoi(split(time1, ':')[1].c_str()), m2 = atoi(split(time1, ':')[1].c_str());
//	cout << day1 << " " << day2 << endl;
	//cout << h1 << " " << m1 << endl;
	if(day1 < day2) {
		return true;
	} else if(day1 == day2) {
		if(h1 < h2)
			return true;
		else if(h1 == h2)
			return m1 <= m2;
		return false;
	} else
		return false;
}

int subtraction_end(string date1, string time1, string date2, string time2) {	//	若a > b，则返回 a - b 正值，否则返回负值 
	int flag = 1;
	if(lessThen_end(date1, time1, date2, time2)) {
		swap(date1, date2);
		swap(time1, time2);
		flag = -1;
	}
//	string date1 = a.startDate, date2 = b.startDate;
//	string time1 = a.startTime, time2 = b.startTime;
//	cout << date1 << endl;
	vector<int> Time1 = getTime(date1, time1), Time2 = getTime(date2, time2);
	int day1 = Time1[0], day2 = Time2[0];
	int h1 = Time1[1], h2 = Time2[1];
	int m1 = Time1[2], m2 = Time2[2];
	
	if(day1 == day2) {
		if(m1 >= m2) {
			return (m1 - m2 + (h1 - h2) * 60) * flag;
		} else {
			return ((h1 - h2) * 60 + m1 - m2) * flag;
		}
	} else {
		int lastday = (day1 - day2) * 24 * 60 - (h2 + 1) * 60 + 60 - m2;
		return (h1 * 60 + m1 + lastday) * flag;
	}
}

int subtraction(fightData a, fightData b) {	//	若a > b，则返回 a - b 正值，否则返回负值 
	int flag = 1;
	if(lessThen(a, b)) {
		swap(a, b);
		flag = -1;
	}
	string date1 = a.startDate, date2 = b.startDate;
	string time1 = a.startTime, time2 = b.startTime;
	cout << date1 << endl;
	vector<int> Time1 = getTime(date1, time1), Time2 = getTime(date2, time2);
	int day1 = Time1[0], day2 = Time2[0];
	int h1 = Time1[1], h2 = Time2[1];
	int m1 = Time1[2], m2 = Time2[2];
	
	if(day1 == day2) {
		if(m1 >= m2) {
			return (m1 - m2 + (h1 - h2) * 60) * flag;
		} else {
			return ((h1 - h2) * 60 + m1 - m2) * flag;
		}
	} else {
		int lastday = (day1 - day2) * 24 * 60 - (h2 + 1) * 60 + 60 - m2;
		return (h1 * 60 + m1 + lastday) * flag;
	}
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
	cout << "s.size() = " << s.size() << endl;
	int number = 0;
	for(set<string>::iterator it = s.begin(); it != s.end(); it++){
		if(number == 0){
			number++;
			continue;
		} 
		// cout << "{name: "<< "\"" << number << ":" << *it << "\" } ]," << endl;
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
	ifstream myfile("机组排班Data A-Crew.csv");
    if( !myfile.is_open()){
    	cout << "Can not open" << endl;
		return;
    }
    string preDataCrew[N];
	for(int i = 0; i < N; i++){
		myfile >> preDataCrew[i];
	}
	for(int i = 1; i <= 21; i++){
		vector<string> value = split(preDataCrew[i], ',');
		memberData[i].empNo = value[0];
		memberData[i].captain = value[1] == "Y" ? 1 : 0;
		memberData[i].firstOfficer = value[2] == "Y" ? 1 : 0;
		memberData[i].deadhead = value[3] == "Y" ? 1 : 0;
		memberData[i].base = value[4];
		memberData[i].dutyCostPerHour = atoi(value[5].c_str());
		memberData[i].paringCostPerHour = atoi(value[6].c_str());
		memberData[i].isUsed = false;
		cout <<"i:" << i << " " <<memberData[i].empNo << " " << memberData[i].captain << endl; 
	}
	cout << "*******" << endl; 
}
 


void init(){
	FILE *fp;
	int i,j ;
    ifstream myfile("机组排班Data A-Flight.csv");
    if( !myfile.is_open()){
    	cout << "Can not open" << endl;
		return;
    }
	for(int i = 0; i < N; i++){
		myfile >> preData[i];
	}
	
	for(int i = 1; i < N; i++){
		splitString(i);
	}
	calUniquePlace();
	
	
}

void calFirst(){
	int startTime = 11;
	int canUsememberNum = 10; 
	sort(data + 1, data + NA, lessThen);
	int lastUpdateNum = 0; 
	for(int i = 1 ; i < NA; i++){
		if(atoi(split(data[i].startDate, '/')[1].c_str()) == startTime){
			for(int j = 1; j <= 10; j++){
				cout << "i = " << i << "ans: " << subtraction_end(data[i].startDate, data[i].startTime, memberData[j].endDate, memberData[j].endTime) << " " << memberData[i].base << endl;
				if(memberData[j].isUsed == false && data[i].startPlace == memberData[j].base && (subtraction_end(data[i].startDate, data[i].startTime, memberData[j].endDate, memberData[j].endTime) >= 40)) {
					memberData[j].isUsed = true;
					memberData[j].endTime = data[i].endTime;
					memberData[j].endDate = data[i].endDate; 
					memberData[j].base = data[i].endPlace;
					data[i].isUsed = true;
					cout << "i2:"<< i << "base:" << memberData[j].base;
					break;
				}
			}
		}
		cout << "i*"<< endl;
		if(i == NA - 1){
			continue; 
		}
		if(atoi(split(data[i + 1].startDate, '/')[1].c_str()) == startTime + 1){
			for(int j = 1; j <= 10; j++){
				memberData[j].isUsed = false;
				memberData[j].endDate = "8/0/2021";
				memberData[j].endTime = "0:0";
			}
		}
	
		for(int j = 1; j <= 10; j++){
			if(memberData[j].isUsed == true){
				//cout << "i5" << i << " " << lessThen_end(memberData[j].endDate, memberData[j].endTime, data[i].startDate, data[i].startTime) << endl;
				cout << memberData[j].endDate << " " <<  memberData[j].endTime << " " << data[i].startDate << " " << data[i].startTime << endl;
				if(lessThen_end(memberData[j].endDate, memberData[j].endTime, data[i + 1].startDate, data[i + 1].startTime)){
					memberData[j].isUsed = false;
				}
			}
		}
		
		if(atoi(split(data[i + 1].startDate, '/')[1].c_str()) == startTime + 1){
			for(int j = lastUpdateNum; j <= i;j++){
				//cout << "j:" << j << " " << data[i].startDate << " " << data[i].startTime << " " << data[i].isUsed << endl;
			}
			lastUpdateNum = i + 1;
			startTime++;
		}
	}
	
	for(int i = 1; i < NA; ++i) {
		cout <<"i = " << i << " "<< data[i].startDate << " " << data[i].startTime << " " << data[i].startPlace <<
		" " <<  data[i].endDate << " " << data[i].endTime << " " << data[i].endPlace << " " <<  data[i].C << " " << data[i].F << " " << data[i].isUsed << endl; 
	}
	
}



int main(){
	init();
	initMember();
	calFirst();
	return 0;
} 




