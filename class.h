#include <bits/stdc++.h>
using namespace std;

struct crewMember {
	string empNo;
	bool captain;
	bool firstOfficer;
	bool deadhead;
	string base;
	string nowBase;
	int dutyCostPerHour;
	int paringCostPerHour;
	bool isUsed;
	string endDate = "8/0/2021";
	string endTime = "0:0";
	int level;	//	1， 2， 3 级，纯副机长从3选，正机长优先选1
	bool haveNight;
	int dutyMin;
};

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
		//cout << this->fltNum << ' ' << this->isUsed << " " << this->endTime << endl;
	}
};

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
	string date1 = a.startDate, date2 = b.startDate;
	string time1 = a.startTime, time2 = b.startTime;
	vector<int> Time1 = getTime(date1, time1), Time2 = getTime(date2, time2);
	int day1 = Time1[0], day2 = Time2[0];
	int h1 = Time1[1], h2 = Time2[1];
	int m1 = Time1[2], m2 = Time2[2];
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
	vector<int> Time1 = getTime(date1, time1), Time2 = getTime(date2, time2);
	int day1 = Time1[0], day2 = Time2[0];
	int h1 = Time1[1], h2 = Time2[1];
	int m1 = Time1[2], m2 = Time2[2];
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
	// cout << date1 << endl;
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