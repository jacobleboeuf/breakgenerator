// C++ program to find difference between two
// given times.
#include <bits/stdc++.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <string>
using namespace std;
ofstream b;
class breakChange {
public:
	breakChange(string bN, string o, string c) {
		bName = bN;
		oldBreak = o;
		changedBreak = c;
	}
	string bName;
	string oldBreak;
	string changedBreak;
};
list<breakChange> breaksToChange, suBreaksToChange, mBreaksToChange,
	tuBreaksToChange, wBreaksToChange, thBreaksToChange, fBreaksToChange,
	saBreaksToChange;
class breakTimes {
public:
	breakTimes(string iStartTime, string iEndTime, string iName, string iDay) {
		name = iName;
		day = iDay;
		startTime = iStartTime;
		endTime = iEndTime;
		needsLunch = false;
		findBreaks();
	}
	breakTimes(
		string iStartTime,
		string iEndTime,
		string iName,
		string iDay,
		bool iIsAdult) {
		name = iName;
		day = iDay;
		startTime = iStartTime;
		endTime = iEndTime;
		isAdult = iIsAdult;
		needsLunch = false;
		findBreaks();
	}
	breakTimes(
		string iStartTime,
		string iEndTime,
		string iName,
		string iDay,
		bool iIsAdult,
		bool iIsTraining) {
		name = iName;
		day = iDay;
		startTime = iStartTime;
		endTime = iEndTime;
		isAdult = iIsAdult;
		isTraining = iIsTraining;
		needsLunch = false;
		findBreaks();
	}
	breakTimes(
		string iStartTime,
		string iEndTime,
		string iName,
		string iDay,
		bool iIsAdult,
		bool iIsShiftLead,
		bool iIsTraining) {
		name = iName;
		day = iDay;
		startTime = iStartTime;
		endTime = iEndTime;
		isAdult = iIsAdult;
		isShiftLead = iIsShiftLead;
		isTraining = iIsTraining;
		needsLunch = false;
		findBreaks();
	}
	breakTimes(
		string iStartTime,
		string iEndTime,
		string iName,
		string iDay,
		bool iIsAdult,
		bool iIsShiftLead,
		bool iIsTraining,
		bool iNeedsLunch,
		list<string> iBreaks) {
		name = iName;
		day = iDay;
		startTime = iStartTime;
		endTime = iEndTime;
		isAdult = iIsAdult;
		isShiftLead = iIsShiftLead;
		isTraining = iIsTraining;
		needsLunch = iNeedsLunch;
		breaks = iBreaks;
	}
	string name, day, startTime, endTime;
	int totalHrs, totalMins;
	list<string> breaks;
	bool isShiftLead, isAdult, isTraining, needsLunch;
	void calcTotalShiftTime();
	void findBreaks();
	// list<breakChange> adjustBreaks(list<breakChange> breaksToChange,
	// list<breakTimes> grids);
	string checkBreak(
		string newBreak,
		list<breakTimes> grids,
		list<breakTimes> shiftLeads,
		list<breakTimes> trainees);
	friend ostream &operator<<(ostream &os, const breakTimes &b);
};
list<breakTimes> suGrids, mGrids, tuGrids, wGrids, thGrids, fGrids, saGrids;
list<breakTimes> suShiftLeads, mShiftLeads, tuShiftLeads, wShiftLeads,
	thShiftLeads, fShiftLeads, saShiftLeads;
list<breakTimes> suAssociates, mAssociates, tuAssociates, wAssociates,
	thAssociates, fAssociates, saAssociates;
list<breakTimes> suBaggers, mBaggers, tuBaggers, wBaggers, thBaggers, fBaggers,
	saBaggers;
list<breakTimes> suTrainees, mTrainees, tuTrainees, wTrainees, thTrainees,
	fTrainees, saTrainees;
ostream &operator<<(ostream &os, const breakTimes &b) {
	os.width(15);
	os << b.name << ":";
	os.width(10);
	os << b.startTime << " - ";
	os.width(5);
	os << b.endTime;
	os.width(10);
	os << "( ";
	for (auto it = b.breaks.cbegin(); it != b.breaks.cend(); it++) {
		os << *it << " ";
	}
	os << ")";
	return os;
}
int getHour(string s) {
	string k = "";
	for (char &c : s) {
		if (c == ':') {
			break;
		}
		k += c;
	}
	return stoi(k);
}
int getMin(string s) {
	string k = "";
	for (char &c : s) {
		if (c == ':') {
			k = "";
		} else {
			k += c;
		}
	}
	return stoi(k);
}
// remove ':' and convert it into an integer
int removeColon(string s) {
	if (s.size() == 4)
		s.replace(1, 1, "");

	if (s.size() == 5)
		s.replace(2, 1, "");

	return stoi(s);
}
string addMin(string s, int minChange) {
	int time = removeColon(s);
	int hourDiff = time / 100;
	int minDiff = time % 100 + minChange;
	if (minDiff >= 60) {
		hourDiff++;
		minDiff = minDiff - 60;
	} else if (minDiff < 0) {
		hourDiff--;
		minDiff = 45;
	}
	if (hourDiff > 12) {
		hourDiff -= 12;
	} else if (hourDiff < 1) {
		hourDiff = 12;
	}
	string res = to_string(hourDiff) + ':' + to_string(minDiff);
	if (minDiff == 0) {
		res += "0";
	}
	return res;
}
bool compareStartTimes(breakTimes b1, breakTimes b2) {
	if (b1.endTime == "10:30") {
		return true;
	} else if (b2.endTime == "10:30") {
		return false;
	}
	int st1 = getHour(b1.startTime);
	int st2 = getHour(b2.startTime);
	if ((st1 < 12 && getHour(b1.endTime) < 12) &&
		(b1.startTime != "6:30" && getHour(b1.endTime) <= 10 &&
		 getHour(b1.endTime) > st1)) {
		st1 += 12;
	}
	if ((st2 < 12 && getHour(b2.endTime) < 12) &&
		(b2.startTime != "6:30" && getHour(b2.endTime) <= 10 &&
		 getHour(b2.endTime) > st2)) {
		st2 += 12;
	}
	int et1 = getHour(b1.endTime);
	int et2 = getHour(b2.endTime);
	if (st1 >= 12 || et1 < getHour(b1.startTime)) {
		et1 += 12;
	}
	if (st2 >= 12 || et2 < getHour(b2.startTime)) {
		et2 += 12;
	}
	if (b1.startTime == b2.startTime) {
		if (b1.endTime == b2.endTime) {
			return false;
		}
		if (et1 > et2) {
			return false;
		} else if (et1 < et2) {
			return true;
		}
		if (getMin(b1.endTime) > getMin(b2.endTime)) {
			return false;
		}
		return true;
	}
	if (st1 > st2) {
		return false;
	} else if (st1 < st2) {
		return true;
	}
	if (getMin(b1.startTime) > getMin(b2.startTime)) {
		return false;
	}
	return true;
}
bool compareEndTimes(breakTimes b1, breakTimes b2) {
	if (getHour(b1.startTime) == 6 &&
		(getHour(b1.endTime) > 10 ||
		 (getHour(b1.endTime) == 10 && getMin(b1.endTime) > 0))) {
		return false;
	} else if (
		getHour(b2.startTime) == 6 &&
		(getHour(b2.endTime) > 10 ||
		 (getHour(b2.endTime) == 10 && getMin(b2.endTime) > 0))) {
		return true;
	}
	int et1 = getHour(b1.endTime);
	int et2 = getHour(b2.endTime);
	int st1 = getHour(b1.startTime);
	int st2 = getHour(b2.startTime);
	if (et1 < 12 &&
		(st1 >= 9 || st1 > et1 || (st1 <= 6 && b1.startTime != "6:30"))) {
		et1 += 12;
	}
	if (et2 < 12 &&
		(st2 >= 9 || st2 > et2 || (st2 <= 6 && b2.startTime != "6:30"))) {
		et2 += 12;
	}
	if (et1 > et2) {
		return true;
	} else {
		return false;
	}
	return true;
}
list<breakTimes>
backTrack(list<breakTimes> bl, list<breakChange> breaksToChange) {
	for (auto p = bl.begin(); p != bl.end(); p++) {
		for (auto q = breaksToChange.begin(); q != breaksToChange.end(); q++) {
			if ((*p).name == (*q).bName && !(*p).isTraining) {
				for (auto r = (*p).breaks.begin(); r != (*p).breaks.end();
					 r++) {
					if (*r == (*q).oldBreak) {
						*r = (*q).changedBreak;
						if ((*p).isTraining) {
							for (auto t = bl.begin(); t != bl.end(); t++) {
								for (auto u = (*t).breaks.begin();
									 u != (*t).breaks.end();
									 u++) {
									if (*r == *u ||
										(*p).startTime == (*t).startTime ||
										(*p).endTime == (*t).endTime) {
										*u = *r;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (bl.front().isShiftLead && bl.size() > 2) {
		int index = 0;
		list<breakTimes>::iterator i = next(bl.begin());
		list<breakTimes>::iterator j;
		if (((*i).startTime == "6:15" || (*i).startTime == "6:45") &&
			(*i).breaks.size() > 0) {
			j = next(i);
			if (getHour((*j).startTime) >= 8 &&
				(bl.front().startTime == "6:30" ||
				 bl.front().startTime == "6:00")) {
				if (getHour((*i).breaks.front()) <= getHour((*j).startTime)) {
					if (getHour((*i).breaks.front()) !=
						getHour((*j).startTime)) {
						while ((*i).breaks.front() != (*j).startTime) {
							(*i).breaks.front() =
								addMin((*i).breaks.front(), 15);
						}
					} else {
						while (getMin((*i).breaks.front()) !=
								   getMin((*j).startTime) &&
							   getHour((*i).breaks.front()) ==
								   getHour((*j).startTime)) {
							if (getHour((*i).breaks.front()) ==
									getHour((*j).startTime) &&
								getMin((*i).breaks.front()) >=
									getMin((*j).startTime)) {
								break;
							}
							(*i).breaks.front() =
								addMin((*i).breaks.front(), 15);
						}
					}
					if (getHour((*i).breaks.front()) > 9 ||
						(getHour((*i).breaks.front()) == 9 &&
						 getMin((*i).breaks.front()) >= 30)) {
						(*i).breaks.front() = "9:15";
					}
				}
			}
		}
		bl.sort(compareEndTimes);
		i = bl.begin(); // i = closing shift lead
		if (((*i).endTime == "9:00" || (*i).endTime == "10:00") &&
			(*i).breaks.size() > 0) {
			j = next(bl.begin()); // j = midshift
			j = next(j);
			j = prev(j);
			if (getHour((*j).endTime) <= 8) {
				if (getHour((*i).breaks.back()) >= getHour((*j).endTime)) {
					if (getHour((*i).breaks.back()) != getHour((*j).endTime)) {
						while ((*i).breaks.back() !=
							   addMin((*j).endTime, -15)) {
							(*i).breaks.back() =
								addMin((*i).breaks.back(), -15);
						}
					} else if (
						getHour((*i).breaks.back()) >
							getHour(addMin((*j).endTime, -15)) ||
						getMin((*i).breaks.back()) >
							getMin(addMin((*j).endTime, -15)) ||
						(*i).breaks.back() == (*j).endTime) {
						while (getMin((*i).breaks.back()) !=
							   getMin(addMin((*j).endTime, -15))) {
							(*i).breaks.back() =
								addMin((*i).breaks.back(), -15);
						}
					}
				}
			}
		}
		bl.sort(compareStartTimes);
	}
	return bl;
}
// Main function which finds difference
int diff(string s1, string s2) {
	// change string (eg. 2:21 --> 221, 00:23 --> 23)
	int time1 = removeColon(s1);
	int time2 = removeColon(s2);

	// difference between hours
	int hourDiff = time1 / 100 - time2 / 100 - 1;
	if (hourDiff < 0) {
		hourDiff = (time1 + 1200) / 100 - time2 / 100 - 1;
	}

	// difference between minutes
	int minDiff = time1 % 100 + (60 - time2 % 100);

	if (minDiff >= 60) {
		hourDiff++;
		minDiff = minDiff - 60;
	}

	// convert answer again in string with ':'
	string res = to_string(hourDiff) + ':' + to_string(minDiff);
	return hourDiff;
}
int countCommas(string s) {
	int count = 0;
	for (int i = 0; i < s.size(); i++) {
		if (s[i] == ',') {
			count++;
		}
	}
	return count;
}
string breakTimes::checkBreak(
	string newBreak,
	list<breakTimes> grids,
	list<breakTimes> shiftLeads,
	list<breakTimes> trainees) {
	string s = newBreak;
	int w = 0;
	int x = 0;
	int y = 0;
	int z = 0;
	bool breakChecked = false;
	bool bft = false;
	bool bnft = false;
	bool bthrt = false;
	if (isTraining && trainees.size() >= 1) {
		for (auto t = trainees.cbegin(); t != trainees.cend(); t++) {
			for (auto u = (*t).breaks.begin(); u != (*t).breaks.end(); u++) {
				if (newBreak == *u || startTime == (*t).startTime ||
					endTime == (*t).endTime) {
					if (newBreak == *u && name != (*t).name) {
						return newBreak;
					} else if (
						addMin(newBreak, 15) == *u ||
						addMin(newBreak, 30) == *u ||
						addMin(newBreak, -15) == *u ||
						addMin(newBreak, -30) == *u) {
						newBreak = *u;
					} else if (
						addMin(newBreak, 15) == (*t).endTime ||
						addMin(newBreak, 30) == (*t).endTime) {
						newBreak = (*t).endTime;
					} else if (
						newBreak == (*t).startTime ||
						addMin(newBreak, 15) == (*t).startTime ||
						addMin(newBreak, 30) == (*t).startTime ||
						addMin(newBreak, -15) == (*t).startTime ||
						addMin(newBreak, -30) == (*t).startTime) {
						newBreak = addMin((*t).startTime, -15);
						if (needsLunch) {
							newBreak = addMin(newBreak, -15);
						}
					}
				}
			}
		}
		return newBreak;
	}
	while (breakChecked == false) {
		for (auto it = grids.begin(); it != grids.end(); it++) {
			int x = 0;
			for (auto p = (*it).breaks.begin(); p != (*it).breaks.end(); p++) {
				x++;
				if ((newBreak == *p ||
					 (x == 2 && newBreak == addMin(*p, 15))) &&
					name != (*it).name) {
					if (x == 2) {
						bft = true;
					}
					if ((endTime != "10:00" && (*it).endTime != "10:00") ||
						(endTime == "10:00" && (*it).endTime == "10:00") ||
						(endTime == "10:00" &&
						 ((*it).endTime != "10:30" && (*it).endTime != "3:00" &&
						  (*it).endTime != "3:15")) ||
						((*it).endTime == "10:00" && endTime != "10:30" &&
						 endTime != "3:00" && (*it).endTime != "3:15")) {
						w++;
						for (auto q = grids.cbegin(); q != grids.cend(); q++) {
							for (auto g = (*q).breaks.begin();
								 g != (*q).breaks.end();
								 g++) {
								if ((*g == addMin(s, 15) ||
									 ((diff(addMin(newBreak, 15), startTime) >=
										   3 &&
									   totalHrs < 6)))) {
									bft = true;
								}
								if ((*g == addMin(s, -15) ||
									 ((diff(newBreak, startTime) == 1 &&
									   (getMin(newBreak) - getMin(startTime) <
										30))))) {
									bnft = true;
								}
								if (*g == addMin(s, 30)) {
									bthrt = true;
								}
							}
						}

						if (!bft) {
							newBreak = addMin(newBreak, 15);
						} else if (!bnft) {
							newBreak = addMin(newBreak, -15);
							if (diff(newBreak, startTime) > 2) {
								newBreak = addMin(newBreak, -15);
							}
						} else if (!bthrt) {
							if (diff(addMin(newBreak, 30), startTime) < 3) {
								newBreak = addMin(newBreak, 30);
							}
						}
					}
				}
			}
		}
		breakChecked = true;
	}
	if (newBreak == s) {
		if (w <= 1) {
			return newBreak;
		} else {
			for (auto q = grids.cbegin(); q != grids.cend(); q++) {
				for (auto g = (*q).breaks.begin(); g != (*q).breaks.end();
					 g++) {
					if (*g == addMin(s, 15)) {
						x++;
					} else if (*g == addMin(s, -15)) {
						y++;
					} else if (*g == addMin(s, 30)) {
						z++;
					}
				}
			}
			if (x <= 1) {
				return addMin(newBreak, 15);
			} else if (y <= 1) {
				return addMin(newBreak, -15);
			} else if (z <= 1) {
				return addMin(newBreak, 30);
			}
		}
	}
	return newBreak;
}
list<breakChange> adjustBreaks(
	breakTimes b,
	list<breakChange> breaksToChange,
	list<breakTimes> grids,
	list<breakTimes> shiftLeads,
	list<breakTimes> trainees) {
	string s = b.name;
	string d = b.day;
	string ob, nb;
	string st = b.startTime;
	string et = b.endTime;
	int th = b.totalHrs;
	bool ia = b.isAdult;
	bool isl = b.isShiftLead;
	bool itr = b.isTraining;
	bool inl = b.needsLunch;
	list<string> bb = b.breaks;
	breakTimes bt(st, et, s, d, ia, isl, itr, inl, bb);
	grids.push_back(bt);
	for (list<breakTimes>::iterator p = grids.begin(); p != grids.end(); p++) {
		for (auto q = (*p).breaks.begin(); q != (*p).breaks.end(); q++) {
			b.name = (*p).name;
			b.startTime = (*p).startTime;
			b.endTime = (*p).endTime;
			b.totalHrs = (*p).totalHrs;
			b.breaks = (*p).breaks;
			ob = *q;
			*q = b.checkBreak(*q, grids, shiftLeads, trainees);
			nb = *q;
			if (ob != nb) {
				breakChange bC((*p).name, ob, nb);
				breaksToChange.push_back(bC);
			}
		}
	}
	b.name = s;
	b.startTime = st;
	b.endTime = et;
	b.totalHrs = th;
	b.breaks = bb;
	return breaksToChange;
}
// Main function which finds difference
void breakTimes::calcTotalShiftTime() {
	// difference between hours
	totalHrs = removeColon(endTime) / 100 - removeColon(startTime) / 100 - 1;
	if (totalHrs < 1) {
		totalHrs = (removeColon(endTime) + 1200) / 100 -
			removeColon(startTime) / 100 - 1;
	}
	// difference between minutes
	totalMins =
		removeColon(endTime) % 100 + (60 - removeColon(startTime) % 100);

	if (totalMins >= 60) {
		totalHrs++;
		totalMins = totalMins - 60;
	}
	if (totalMins >= 7) {
		needsLunch = true;
	}
}
ofstream printGrids(list<breakTimes> l, ofstream o) {
	o << "***************************************************************"
	  << endl
	  << endl;
	for (auto it = l.cbegin(); it != l.cend(); it++) {
		o << *it << endl;
	}
	o << endl
	  << "***************************************************************"
	  << endl;
	return o;
}
void breakTimes::findBreaks() {
	calcTotalShiftTime();
	string newBreak = "X";
	string s = startTime;
	int breakHr = -2;
	int breakMin = 0;
	if ((totalHrs < 4 && isAdult) || (totalHrs < 3) ||
		(totalHrs == 3 && totalMins < 30) || name == "Diane") {
		// Shift doesn't need break
		return;
	} else if (totalHrs < 6) {
		// Shift with only 1 break
		breakHr = removeColon(startTime) / 100 + (totalHrs / 2);
		if (!isAdult && totalHrs < 4 && totalMins > 30) {
			breakHr = removeColon(startTime) / 100 + 2;
		} else if ((totalMins >= 30) && (totalHrs == 5)) {
			breakMin = 45;
		} else if (totalMins == 30) {
			breakMin = 15;
		} else if ((totalHrs == 5 && totalMins < 30) || totalMins == 45) {
			breakMin = 30;
		}
		breakMin = removeColon(startTime) % 100 + breakMin;
		if (breakMin >= 60) {
			breakHr++;
			breakMin -= 60;
		}
		if (breakHr > 12) {
			breakHr -= 12;
		}
		newBreak = to_string(breakHr) + ":";
		if (breakMin == 0) {
			newBreak += "00";
		} else {
			newBreak += to_string(breakMin);
		}
		if (day == "Sun" || day == "Sunday" || day == "sun" ||
			day == "sunday" || day == "Su" || day == "su") {
			newBreak = checkBreak(newBreak, suGrids, suShiftLeads, suTrainees);
		} else if (
			day == "Mon" || day == "Monday" || day == "mon" ||
			day == "monday" || day == "Mo" || day == "mo") {
			newBreak = checkBreak(newBreak, mGrids, mShiftLeads, mTrainees);
		} else if (
			day == "Tues" || day == "Tuesday" || day == "tues" ||
			day == "tuesday" || day == "Tu" || day == "tu") {
			newBreak = checkBreak(newBreak, tuGrids, tuShiftLeads, tuTrainees);
		} else if (
			day == "Wed" || day == "Wednesday" || day == "wed" ||
			day == "wednesday" || day == "We" || day == "we") {
			newBreak = checkBreak(newBreak, wGrids, wShiftLeads, wTrainees);
		} else if (
			day == "Thurs" || day == "Thursday" || day == "thurs" ||
			day == "thursday" || day == "Th" || day == "th") {
			newBreak = checkBreak(newBreak, thGrids, thShiftLeads, thTrainees);
		} else if (
			day == "Fri" || day == "Friday" || day == "fri" ||
			day == "friday" || day == "Fr" || day == "fr") {
			newBreak = checkBreak(newBreak, fGrids, fShiftLeads, fTrainees);
		} else if (
			day == "Sat" || day == "Saturday" || day == "sat" ||
			day == "saturday" || day == "Sa" || day == "sa") {
			newBreak = checkBreak(newBreak, saGrids, saShiftLeads, saTrainees);
		}
		breaks.push_back(newBreak);
	} else if (totalHrs == 6) {
		// 6 Hour shift, need break and a lunch
		s = startTime;
		breakHr = 2;
		for (int i = 0; i < 2; i++) {
			breakHr = removeColon(s) / 100 + breakHr;
			breakMin += removeColon(s) % 100;
			if (breakMin >= 60) {
				breakHr++;
				breakMin -= 60;
			}
			if (breakHr > 12) {
				breakHr -= 12;
			}
			newBreak = to_string(breakHr) + ":";
			if (breakMin == 0) {
				newBreak += "00";
			} else {
				newBreak += to_string(breakMin);
			}
			if (diff(endTime, newBreak) == 2) {
				int x = breakMin;
				while (totalMins > x) {
					newBreak = addMin(newBreak, 15);
					x += 15;
				}
			}
			if (day == "Sun" || day == "Sunday" || day == "sun" ||
				day == "sunday" || day == "Su" || day == "su") {
				newBreak =
					checkBreak(newBreak, suGrids, suShiftLeads, suTrainees);
			} else if (
				day == "Mon" || day == "Monday" || day == "mon" ||
				day == "monday" || day == "Mo" || day == "mo") {
				newBreak = checkBreak(newBreak, mGrids, mShiftLeads, mTrainees);
			} else if (
				day == "Tues" || day == "Tuesday" || day == "tues" ||
				day == "tuesday" || day == "Tu" || day == "tu") {
				newBreak =
					checkBreak(newBreak, tuGrids, tuShiftLeads, tuTrainees);
			} else if (
				day == "Wed" || day == "Wednesday" || day == "wed" ||
				day == "wednesday" || day == "We" || day == "we") {
				newBreak = checkBreak(newBreak, wGrids, wShiftLeads, wTrainees);
			} else if (
				day == "Thurs" || day == "Thursday" || day == "thurs" ||
				day == "thursday" || day == "Th" || day == "th") {
				newBreak =
					checkBreak(newBreak, thGrids, thShiftLeads, thTrainees);
			} else if (
				day == "Fri" || day == "Friday" || day == "fri" ||
				day == "friday" || day == "Fr" || day == "fr") {
				newBreak = checkBreak(newBreak, fGrids, fShiftLeads, fTrainees);
			} else if (
				day == "Sat" || day == "Saturday" || day == "sat" ||
				day == "saturday" || day == "Sa" || day == "sa") {
				newBreak =
					checkBreak(newBreak, saGrids, saShiftLeads, saTrainees);
			}
			breaks.push_back(newBreak);
			s = newBreak;
			breakHr = 2;
			if (totalMins < 45) {
				breakMin = 0;
			} else {
				breakMin = 15;
			}
		}
	} else if (totalHrs == 7 && totalMins <= 30) {
		// 7 Hour shift
		breakMin = 30;
		if (totalMins == 30) {
			breakMin = 45;
		}
		breakHr = 1;
		for (int i = 0; i < 3; i++) {
			breakHr = removeColon(s) / 100 + breakHr;
			breakMin = removeColon(s) % 100 + breakMin;
			if (breakMin >= 60) {
				breakHr++;
				breakMin -= 60;
			}
			if (breakHr > 12) {
				breakHr -= 12;
			}
			newBreak = to_string(breakHr) + ":";
			if (breakMin == 0) {
				newBreak += "00";
			} else {
				newBreak += to_string(breakMin);
			}
			if (day == "Sun" || day == "Sunday" || day == "sun" ||
				day == "sunday" || day == "Su" || day == "su") {
				newBreak =
					checkBreak(newBreak, suGrids, suShiftLeads, suTrainees);
			} else if (
				day == "Mon" || day == "Monday" || day == "mon" ||
				day == "monday" || day == "Mo" || day == "mo") {
				newBreak = checkBreak(newBreak, mGrids, mShiftLeads, mTrainees);
			} else if (
				day == "Tues" || day == "Tuesday" || day == "tues" ||
				day == "tuesday" || day == "Tu" || day == "tu") {
				newBreak =
					checkBreak(newBreak, tuGrids, tuShiftLeads, tuTrainees);
			} else if (
				day == "Wed" || day == "Wednesday" || day == "wed" ||
				day == "wednesday" || day == "We" || day == "we") {
				newBreak = checkBreak(newBreak, wGrids, wShiftLeads, wTrainees);
			} else if (
				day == "Thurs" || day == "Thursday" || day == "thurs" ||
				day == "thursday" || day == "Th" || day == "th") {
				newBreak =
					checkBreak(newBreak, thGrids, thShiftLeads, thTrainees);
			} else if (
				day == "Fri" || day == "Friday" || day == "fri" ||
				day == "friday" || day == "Fr" || day == "fr") {
				newBreak = checkBreak(newBreak, fGrids, fShiftLeads, fTrainees);
			} else if (
				day == "Sat" || day == "Saturday" || day == "sat" ||
				day == "saturday" || day == "Sa" || day == "sa") {
				newBreak =
					checkBreak(newBreak, saGrids, saShiftLeads, saTrainees);
			}
			breaks.push_back(newBreak);
			s = newBreak;
			breakMin = 0;
			breakHr = 2;
		}
	} else if (
		totalHrs == 7 || (totalHrs == 8 && totalMins < 15) || totalHrs > 9) {
		// Long 7 hour shift / Shorter 8 hour shift / 10 hour shift
		int i = 1;
		if (totalHrs > 9) {
			i = 0;
		}
		while (i < 4) {
			breakHr = 2;
			breakHr = removeColon(s) / 100 + breakHr;
			breakMin = removeColon(s) % 100;
			if (breakHr > 12) {
				breakHr -= 12;
			}
			newBreak = to_string(breakHr) + ":";
			if (breakMin == 0) {
				newBreak += "00";
			} else {
				newBreak += to_string(breakMin);
			}
			if (day == "Sun" || day == "Sunday" || day == "sun" ||
				day == "sunday" || day == "Su" || day == "su") {
				newBreak =
					checkBreak(newBreak, suGrids, suShiftLeads, suTrainees);
			} else if (
				day == "Mon" || day == "Monday" || day == "mon" ||
				day == "monday" || day == "Mo" || day == "mo") {
				newBreak = checkBreak(newBreak, mGrids, mShiftLeads, mTrainees);
			} else if (
				day == "Tues" || day == "Tuesday" || day == "tues" ||
				day == "tuesday" || day == "Tu" || day == "tu") {
				newBreak =
					checkBreak(newBreak, tuGrids, tuShiftLeads, tuTrainees);
			} else if (
				day == "Wed" || day == "Wednesday" || day == "wed" ||
				day == "wednesday" || day == "We" || day == "we") {
				newBreak = checkBreak(newBreak, wGrids, wShiftLeads, wTrainees);
			} else if (
				day == "Thurs" || day == "Thursday" || day == "thurs" ||
				day == "thursday" || day == "Th" || day == "th") {
				newBreak =
					checkBreak(newBreak, thGrids, thShiftLeads, thTrainees);
			} else if (
				day == "Fri" || day == "Friday" || day == "fri" ||
				day == "friday" || day == "Fr" || day == "fr") {
				newBreak = checkBreak(newBreak, fGrids, fShiftLeads, fTrainees);
			} else if (
				day == "Sat" || day == "Saturday" || day == "sat" ||
				day == "saturday" || day == "Sa" || day == "sa") {
				newBreak =
					checkBreak(newBreak, saGrids, saShiftLeads, saTrainees);
			}
			breaks.push_back(newBreak);
			s = newBreak;
			i++;
		}
	} else {
		// All other shifts requiring 3 breaks
		s = endTime;
		int mins = totalMins;
		if (totalHrs > 8) {
			mins += 45;
		}
		for (int i = 0; i < 3; i++) {
			breakHr = removeColon(s) / 100 + breakHr - 1;
			breakMin = removeColon(s) % 100 + (60 - breakMin % 100);
			if (breakMin >= 60) {
				breakHr++;
				breakMin -= 60;
			}
			if (breakHr > 12) {
				breakHr -= 12;
			}
			if (breakHr < 1) {
				breakHr += 12;
			}
			newBreak = to_string(breakHr) + ":";
			if (breakMin == 0) {
				newBreak += "00";
			} else {
				newBreak += to_string(breakMin);
			}
			if (day == "Sun" || day == "Sunday" || day == "sun" ||
				day == "sunday" || day == "Su" || day == "su") {
				newBreak =
					checkBreak(newBreak, suGrids, suShiftLeads, suTrainees);
			} else if (
				day == "Mon" || day == "Monday" || day == "mon" ||
				day == "monday" || day == "Mo" || day == "mo") {
				newBreak = checkBreak(newBreak, mGrids, mShiftLeads, mTrainees);
			} else if (
				day == "Tues" || day == "Tuesday" || day == "tues" ||
				day == "tuesday" || day == "Tu" || day == "tu") {
				newBreak =
					checkBreak(newBreak, tuGrids, tuShiftLeads, tuTrainees);
			} else if (
				day == "Wed" || day == "Wednesday" || day == "wed" ||
				day == "wednesday" || day == "We" || day == "we") {
				newBreak = checkBreak(newBreak, wGrids, wShiftLeads, wTrainees);
			} else if (
				day == "Thurs" || day == "Thursday" || day == "thurs" ||
				day == "thursday" || day == "Th" || day == "th") {
				newBreak =
					checkBreak(newBreak, thGrids, thShiftLeads, thTrainees);
			} else if (
				day == "Fri" || day == "Friday" || day == "fri" ||
				day == "friday" || day == "Fr" || day == "fr") {
				newBreak = checkBreak(newBreak, fGrids, fShiftLeads, fTrainees);
			} else if (
				day == "Sat" || day == "Saturday" || day == "sat" ||
				day == "saturday" || day == "Sa" || day == "sa") {
				newBreak =
					checkBreak(newBreak, saGrids, saShiftLeads, saTrainees);
			}
			breaks.push_front(newBreak);
			s = newBreak;
			if ((mins - 30) < 0) {
				if ((mins - 15) >= 0) {
					mins -= 15;
					breakMin = 15;
				} else {
					breakMin = 0;
				}
			} else {
				mins -= 30;
				breakMin = 30;
				if (mins >= 30) {
					breakMin += 15;
				}
			}
			breakHr = -2;
		}
	}
}
// Driver program to test above functions
int main() {
	string input, day, name, sTime, eTime, filename, yn;
	char a;
	bool isAdult = true, training = false, shiftLead = false;
	bool notCashier = false;
	cout << endl << "Enter week of grids (i.e. weekending March21): ";
	cin >> filename;
	while (input != "Q" && input != "q") {
		cout << endl
			 << "Enter shift times in format [Can't cashier(if yes, 'y'), "
				"name, under 18?(if yes, 'y'), day, shift leader?(if yes, 'y') "
				"start time, end time, training?(if yes, 'y'), day, start "
				"time, end time, training?(if yes, 'y'),..., or enter q to quit"
			 << endl;
		cin >> input;
		if (input != "Q" && input != "q") {
			if (countCommas(input) < 3) {
				cout << "Too small!" << endl;
				break;
			}
			yn = input.substr(0, input.find_first_of(","));
			if (yn.size() == 1) {
				if (yn == "y" || yn == "Y") {
					notCashier = true;
					input = input.substr(input.find_first_of(",") + 1);
				}
			}

			name = input.substr(0, input.find_first_of(","));
			input = input.substr(input.find_first_of(",") + 1);
			yn = input.substr(0, input.find_first_of(","));
			if (yn.size() > 1) {
				day = yn;
			} else {
				if (yn == "y" || yn == "Y") {
					isAdult = false;
				}
				input = input.substr(input.find_first_of(",") + 1);
				day = input.substr(0, input.find_first_of(","));
			}
			input = input.substr(input.find_first_of(",") + 1);
			yn = input.substr(0, input.find_first_of(","));
			if (yn.size() > 1) {
				sTime = yn;
        cout << input << endl;
			} else {
				if (yn == "y" || yn == "Y") {
					shiftLead = true;
				}
				input = input.substr(input.find_first_of(",") + 1);
				sTime = input.substr(0, input.find_first_of(","));
        cout << input << endl;
			}
			input = input.substr(input.find_first_of(",") + 1);
			if (countCommas(input) > 0) {
				eTime = input.substr(0, input.find_first_of(","));
        cout << input << endl;
				input = input.substr(input.find_first_of(",") + 1);
				if (input.size() > 1) {
					yn = input.substr(0, input.find_first_of(","));
					if (yn.size() == 1) {
						yn = input.substr(0, input.find_first_of(","));
						input = input.substr(input.find_first_of(",") + 1);
					}
				} else {
					yn = input;
				}
				if (yn == "y" || yn == "Y") {
					training = true;
				}
			} else {
				eTime = input;
			}
			breakTimes x(sTime, eTime, name, day, isAdult, shiftLead, training);
			if (day == "sunday" || day == "sun" || day == "Sunday" ||
				day == "Sun" || day == "Su" || day == "su") {
				suBreaksToChange = adjustBreaks(
					x, suBreaksToChange, suGrids, suShiftLeads, suTrainees);
				if (training) {
					suTrainees.push_back(x);
				} else if (shiftLead) {
					suShiftLeads.push_back(x);
				} else if (notCashier) {
					suBaggers.push_back(x);
				} else {
					suAssociates.push_back(x);
				}
				suGrids.push_back(x);
			} else if (
				day == "monday" || day == "mon" || day == "Monday" ||
				day == "Mon" || day == "Mo" || day == "mo") {
				mBreaksToChange = adjustBreaks(
					x, mBreaksToChange, mGrids, mShiftLeads, mTrainees);
				if (training) {
					mTrainees.push_back(x);
				} else if (shiftLead) {
					mShiftLeads.push_back(x);
				} else if (notCashier) {
					mBaggers.push_back(x);
				} else {
					mAssociates.push_back(x);
				}
				mGrids.push_back(x);
			} else if (
				day == "tuesday" || day == "tues" || day == "Tuesday" ||
				day == "Tues" || day == "Tu" || day == "tu") {
				tuBreaksToChange = adjustBreaks(
					x, tuBreaksToChange, tuGrids, tuShiftLeads, tuTrainees);
				if (training) {
					tuTrainees.push_back(x);
				} else if (shiftLead) {
					tuShiftLeads.push_back(x);
				} else if (notCashier) {
					tuBaggers.push_back(x);
				} else {
					tuAssociates.push_back(x);
				}
				tuGrids.push_back(x);
			} else if (
				day == "wednesday" || day == "wed" || day == "Wednesday" ||
				day == "Wed" || day == "We" || day == "we") {
				wBreaksToChange = adjustBreaks(
					x, wBreaksToChange, wGrids, wShiftLeads, wTrainees);
				if (training) {
					wTrainees.push_back(x);
				} else if (shiftLead) {
					wShiftLeads.push_back(x);
				} else if (notCashier) {
					wBaggers.push_back(x);
				} else {
					wAssociates.push_back(x);
				}
				wGrids.push_back(x);
			} else if (
				day == "thursday" || day == "thurs" || day == "Thursday" ||
				day == "Thurs" || day == "Th" || day == "th") {
				thBreaksToChange = adjustBreaks(
					x, thBreaksToChange, thGrids, thShiftLeads, thTrainees);
				if (training) {
					thTrainees.push_back(x);
				} else if (shiftLead) {
					thShiftLeads.push_back(x);
				} else if (notCashier) {
					thBaggers.push_back(x);
				} else {
					thAssociates.push_back(x);
				}
				thGrids.push_back(x);
			} else if (
				day == "friday" || day == "fri" || day == "Friday" ||
				day == "Fri" || day == "Fr" || day == "fr") {
				fBreaksToChange = adjustBreaks(
					x, fBreaksToChange, fGrids, fShiftLeads, fTrainees);
				if (training) {
					fTrainees.push_back(x);
				} else if (shiftLead) {
					fShiftLeads.push_back(x);
				} else if (notCashier) {
					fBaggers.push_back(x);
				} else {
					fAssociates.push_back(x);
				}
				fGrids.push_back(x);
			} else if (
				day == "saturday" || day == "sat" || day == "Saturday" ||
				day == "Sat" || day == "Sa" || day == "sa") {
				saBreaksToChange = adjustBreaks(
					x, saBreaksToChange, saGrids, saShiftLeads, saTrainees);
				if (training) {
					saTrainees.push_back(x);
				} else if (shiftLead) {
					saShiftLeads.push_back(x);
				} else if (notCashier) {
					saBaggers.push_back(x);
				} else {
					saAssociates.push_back(x);
				}
				saGrids.push_back(x);
			}
      cout << input << endl;
			while (countCommas(input) >= 2) {
				day = input.substr(0, input.find_first_of(","));
				input = input.substr(input.find_first_of(",") + 1);
				sTime = input.substr(0, input.find_first_of(","));
				input = input.substr(input.find_first_of(",") + 1);
        cout << sTime << endl;
				if (countCommas(input) > 0) {
					eTime = input.substr(0, input.find_first_of(","));
					input = input.substr(input.find_first_of(",") + 1);
          cout << eTime << endl;
					if (input.size() > 1) {
						yn = input.substr(0, input.find_first_of(","));
						if (yn.size() == 1) {
							yn = input.substr(0, input.find_first_of(","));
							input = input.substr(input.find_first_of(",") + 1);
						}
					} else {
						yn = input;
					}
					if (yn == "y" || yn == "Y") {
						training = true;
					}
				} else {
					eTime = input;
				}
				breakTimes t(
					sTime, eTime, name, day, isAdult, shiftLead, training);
				if (day == "sunday" || day == "sun" || day == "Sunday" ||
					day == "Sun" || day == "Su" || day == "su") {
					suBreaksToChange = adjustBreaks(
						t, suBreaksToChange, suGrids, suShiftLeads, suTrainees);
					suBreaksToChange = adjustBreaks(
						t,
						suBreaksToChange,
						suShiftLeads,
						suShiftLeads,
						suTrainees);
					if (training) {
						suTrainees.push_back(t);
					} else if (shiftLead) {
						suShiftLeads.push_back(t);
					} else if (notCashier) {
						suBaggers.push_back(x);
					} else {
						suAssociates.push_back(t);
					}
					suGrids.push_back(t);
				} else if (
					day == "monday" || day == "mon" || day == "Monday" ||
					day == "Mon" || day == "Mo" || day == "mo") {
					mBreaksToChange = adjustBreaks(
						t, mBreaksToChange, mGrids, mShiftLeads, mTrainees);
					mBreaksToChange = adjustBreaks(
						t,
						mBreaksToChange,
						mShiftLeads,
						mShiftLeads,
						mTrainees);
					if (training) {
						mTrainees.push_back(t);
					} else if (shiftLead) {
						mShiftLeads.push_back(t);
					} else if (notCashier) {
						mBaggers.push_back(x);
					} else {
						mAssociates.push_back(t);
					}
					mGrids.push_back(t);
				} else if (
					day == "tuesday" || day == "tues" || day == "Tuesday" ||
					day == "Tues" || day == "Tu" || day == "tu") {
					tuBreaksToChange = adjustBreaks(
						t, tuBreaksToChange, tuGrids, tuShiftLeads, tuTrainees);
					tuBreaksToChange = adjustBreaks(
						t,
						tuBreaksToChange,
						tuShiftLeads,
						tuShiftLeads,
						tuTrainees);
					if (training) {
						tuTrainees.push_back(t);
					} else if (shiftLead) {
						tuShiftLeads.push_back(t);
					} else if (notCashier) {
						tuBaggers.push_back(x);
					} else {
						tuAssociates.push_back(t);
					}
					tuGrids.push_back(t);
				} else if (
					day == "wednesday" || day == "wed" || day == "Wednesday" ||
					day == "Wed" || day == "We" || day == "we") {
					wBreaksToChange = adjustBreaks(
						t, wBreaksToChange, wGrids, wShiftLeads, wTrainees);
					wBreaksToChange = adjustBreaks(
						t,
						wBreaksToChange,
						wShiftLeads,
						wShiftLeads,
						wTrainees);
					if (training) {
						wTrainees.push_back(t);
					} else if (shiftLead) {
						wShiftLeads.push_back(t);
					} else if (notCashier) {
						wBaggers.push_back(x);
					} else {
						wAssociates.push_back(t);
					}
					wGrids.push_back(t);
				} else if (
					day == "thursday" || day == "thurs" || day == "Thursday" ||
					day == "Thurs" || day == "Th" || day == "th") {
					thBreaksToChange = adjustBreaks(
						t, thBreaksToChange, thGrids, thShiftLeads, thTrainees);
					thBreaksToChange = adjustBreaks(
						t,
						thBreaksToChange,
						thShiftLeads,
						thShiftLeads,
						thTrainees);
					if (training) {
						thTrainees.push_back(t);
					} else if (shiftLead) {
						thShiftLeads.push_back(t);
					} else if (notCashier) {
						thBaggers.push_back(x);
					} else {
						thAssociates.push_back(t);
					}
					thGrids.push_back(t);
				} else if (
					day == "friday" || day == "fri" || day == "Friday" ||
					day == "Fri" || day == "Fr" || day == "fr") {
					fBreaksToChange = adjustBreaks(
						t, fBreaksToChange, fGrids, fShiftLeads, fTrainees);
					fBreaksToChange = adjustBreaks(
						t,
						fBreaksToChange,
						fShiftLeads,
						fShiftLeads,
						fTrainees);
					if (training) {
						fTrainees.push_back(t);
					} else if (shiftLead) {
						fShiftLeads.push_back(t);
					} else if (notCashier) {
						fBaggers.push_back(x);
					} else {
						fAssociates.push_back(t);
					}
					fGrids.push_back(t);
				} else if (
					day == "saturday" || day == "sat" || day == "Saturday" ||
					day == "Sat" || day == "Sa" || day == "sa") {
					saBreaksToChange = adjustBreaks(
						t, saBreaksToChange, saGrids, saShiftLeads, saTrainees);
					saBreaksToChange = adjustBreaks(
						t,
						saBreaksToChange,
						saShiftLeads,
						saShiftLeads,
						saTrainees);
					if (training) {
						saTrainees.push_back(t);
					} else if (shiftLead) {
						saShiftLeads.push_back(t);
					} else if (notCashier) {
						saBaggers.push_back(x);
					} else {
						saAssociates.push_back(t);
					}
					saGrids.push_back(t);
				}
				training = false;
			}
			isAdult = true;
			training = false;
			shiftLead = false;
			notCashier = false;
		}
	}
	b.open("w-e" + filename + "/" + filename + "-1sungrids.txt");
	suAssociates.sort(compareStartTimes);
	suBaggers.sort(compareStartTimes);
	suShiftLeads.sort(compareStartTimes);
	suTrainees.sort(compareStartTimes);
	suAssociates = backTrack(suAssociates, suBreaksToChange);
	suBaggers = backTrack(suBaggers, suBreaksToChange);
	suShiftLeads = backTrack(suShiftLeads, suBreaksToChange);
	suTrainees = backTrack(suTrainees, suBreaksToChange);
	/*suBreaksToChange.clear();
	for (auto p = suAssociates.begin(); p != suAssociates.end(); p++) {
	  suBreaksToChange = adjustBreaks(*p, suBreaksToChange, suGrids,
	suShiftLeads, suTrainees);
	}
	suAssociates = backTrack(suAssociates, suBreaksToChange);
	 for (auto p = suShiftLeads.begin(); p != suShiftLeads.end(); p++) {
	  suBreaksToChange = adjustBreaks(*p, suBreaksToChange, suGrids,
	suShiftLeads, suTrainees);
	}
	suShiftLeads = backTrack(suShiftLeads, suBreaksToChange);
	 for (auto p = suTrainees.begin(); p != suTrainees.end(); p++) {
	  suBreaksToChange = adjustBreaks(*p, suBreaksToChange, suGrids,
	suShiftLeads, suTrainees);
	}
	suTrainees = backTrack(suTrainees, suBreaksToChange);
	*/
	b << "Sunday (Weekending " << filename << ")" << endl;
	b << "***************************************************************"
	  << endl
	  << "Cashier Trained Associates" << endl;
	b << "***************************************************************"
	  << endl
	  << endl;
	for (auto it = suAssociates.cbegin(); it != suAssociates.cend(); it++) {
		b << *it << endl;
	}
	b << endl
	  << "***************************************************************"
	  << endl;
	b << "***************************************************************"
	  << endl
	  << "Baggers/Service Clerks" << endl;
	b << "***************************************************************"
	  << endl
	  << endl;
	for (auto it = suBaggers.cbegin(); it != suBaggers.cend(); it++) {
		b << *it << endl;
	}
	b << endl
	  << "***************************************************************"
	  << endl;
	b << "***************************************************************"
	  << endl
	  << "Service Leaders" << endl;
	b << "***************************************************************"
	  << endl
	  << endl;
	for (auto it = suShiftLeads.cbegin(); it != suShiftLeads.cend(); it++) {
		b << *it << endl;
	}
	b << endl
	  << "***************************************************************"
	  << endl;
	b << "***************************************************************"
	  << endl
	  << "Training" << endl;
	b << "***************************************************************"
	  << endl
	  << endl;
	for (auto it = suTrainees.cbegin(); it != suTrainees.cend(); it++) {
		b << *it << endl;
	}
	b << endl
	  << "***************************************************************"
	  << endl;
	b.close();
	b.open("w-e" + filename + "/" + filename + "-2mongrids.txt");
	mAssociates.sort(compareStartTimes);
	mBaggers.sort(compareStartTimes);
	mShiftLeads.sort(compareStartTimes);
	mTrainees.sort(compareStartTimes);
	mAssociates = backTrack(mAssociates, mBreaksToChange);
	mBaggers = backTrack(mBaggers, mBreaksToChange);
	mShiftLeads = backTrack(mShiftLeads, mBreaksToChange);
	mTrainees = backTrack(mTrainees, mBreaksToChange);
	/*mBreaksToChange.clear();
	for (auto p = mAssociates.begin(); p != mAssociates.end(); p++) {
	  mBreaksToChange = adjustBreaks(*p, mBreaksToChange, mGrids, mShiftLeads,
	mTrainees);
	}
	mAssociates = backTrack(mAssociates, mBreaksToChange);
	 for (auto p = mShiftLeads.begin(); p != mShiftLeads.end(); p++) {
	  mBreaksToChange = adjustBreaks(*p, mBreaksToChange, mGrids, mShiftLeads,
	mTrainees);
	}
	mShiftLeads = backTrack(mShiftLeads, mBreaksToChange);
	 for (auto p = mTrainees.begin(); p != mTrainees.end(); p++) {
	  mBreaksToChange = adjustBreaks(*p, mBreaksToChange, mGrids, mShiftLeads,
	mTrainees);
	}
	mTrainees = backTrack(mTrainees, mBreaksToChange);
	*/
	b << "Monday (Weekending " << filename << ")" << endl;
	b << "***************************************************************"
	  << endl
	  << "Cashier Trained Associates" << endl;
	b << "***************************************************************"
	  << endl
	  << endl;
	for (auto it = mAssociates.cbegin(); it != mAssociates.cend(); it++) {
		b << *it << endl;
	}
	b << endl
	  << "***************************************************************"
	  << endl;
	b << "***************************************************************"
	  << endl
	  << "Baggers/Service Clerks" << endl;
	b << "***************************************************************"
	  << endl
	  << endl;
	for (auto it = mBaggers.cbegin(); it != mBaggers.cend(); it++) {
		b << *it << endl;
	}
	b << endl
	  << "***************************************************************"
	  << endl;
	b << "***************************************************************"
	  << endl
	  << "Service Leaders" << endl;
	b << "***************************************************************"
	  << endl
	  << endl;
	for (auto it = mShiftLeads.cbegin(); it != mShiftLeads.cend(); it++) {
		b << *it << endl;
	}
	b << endl
	  << "***************************************************************"
	  << endl;
	b << "***************************************************************"
	  << endl
	  << "Training" << endl;
	b << "***************************************************************"
	  << endl
	  << endl;
	for (auto it = mTrainees.cbegin(); it != mTrainees.cend(); it++) {
		b << *it << endl;
	}
	b << endl
	  << "***************************************************************"
	  << endl;
	b.close();
	b.open("w-e" + filename + "/" + filename + "-3tuesgrids.txt");
	tuAssociates.sort(compareStartTimes);
	tuBaggers.sort(compareStartTimes);
	tuShiftLeads.sort(compareStartTimes);
	tuTrainees.sort(compareStartTimes);
	tuAssociates = backTrack(tuAssociates, tuBreaksToChange);
	tuBaggers = backTrack(tuBaggers, tuBreaksToChange);
	tuShiftLeads = backTrack(tuShiftLeads, tuBreaksToChange);
	tuTrainees = backTrack(tuTrainees, tuBreaksToChange);
	/*
	tuBreaksToChange.clear();
	for (auto p = tuAssociates.begin(); p != tuAssociates.end(); p++) {
	  tuBreaksToChange = adjustBreaks(*p, tuBreaksToChange, tuGrids,
	tuShiftLeads, tuTrainees);
	}
	tuAssociates = backTrack(tuAssociates, tuBreaksToChange);
	 for (auto p = tuShiftLeads.begin(); p != tuShiftLeads.end(); p++) {
	  tuBreaksToChange = adjustBreaks(*p, tuBreaksToChange, tuGrids,
	tuShiftLeads, tuTrainees);
	}
	tuShiftLeads = backTrack(tuShiftLeads, tuBreaksToChange);
	 for (auto p = tuTrainees.begin(); p != tuTrainees.end(); p++) {
	  tuBreaksToChange = adjustBreaks(*p, tuBreaksToChange, tuGrids,
	tuShiftLeads, tuTrainees);
	}
	tuTrainees = backTrack(tuTrainees, tuBreaksToChange);
	*/
	b << "Tuesday (Weekending " << filename << ")" << endl;
	b << "***************************************************************"
	  << endl
	  << "Cashier Trained Associates" << endl;
	b << "***************************************************************"
	  << endl
	  << endl;
	for (auto it = tuAssociates.cbegin(); it != tuAssociates.cend(); it++) {
		b << *it << endl;
	}
	b << endl
	  << "***************************************************************"
	  << endl;
	b << "***************************************************************"
	  << endl
	  << "Baggers/Service Clerks" << endl;
	b << "***************************************************************"
	  << endl
	  << endl;
	for (auto it = tuBaggers.cbegin(); it != tuBaggers.cend(); it++) {
		b << *it << endl;
	}
	b << endl
	  << "***************************************************************"
	  << endl;
	b << "***************************************************************"
	  << endl
	  << "Service Leaders" << endl;
	b << "***************************************************************"
	  << endl
	  << endl;
	for (auto it = tuShiftLeads.cbegin(); it != tuShiftLeads.cend(); it++) {
		b << *it << endl;
	}
	b << endl
	  << "***************************************************************"
	  << endl;
	b << "***************************************************************"
	  << endl
	  << "Training" << endl;
	b << "***************************************************************"
	  << endl
	  << endl;
	for (auto it = tuTrainees.cbegin(); it != tuTrainees.cend(); it++) {
		b << *it << endl;
	}
	b << endl
	  << "***************************************************************"
	  << endl;
	b.close();
	b.open("w-e" + filename + "/" + filename + "-4wedgrids.txt");
	wAssociates.sort(compareStartTimes);
	wBaggers.sort(compareStartTimes);
	wShiftLeads.sort(compareStartTimes);
	wTrainees.sort(compareStartTimes);
	wAssociates = backTrack(wAssociates, wBreaksToChange);
	wBaggers = backTrack(wBaggers, wBreaksToChange);
	wShiftLeads = backTrack(wShiftLeads, wBreaksToChange);
	wTrainees = backTrack(wTrainees, wBreaksToChange);
	/*
	wBreaksToChange.clear();
	for (auto p = wAssociates.begin(); p != wAssociates.end(); p++) {
	  wBreaksToChange = adjustBreaks(*p, wBreaksToChange, wGrids, wShiftLeads,
	wTrainees);
	}
	wAssociates = backTrack(wAssociates, wBreaksToChange);
	 for (auto p = wShiftLeads.begin(); p != wShiftLeads.end(); p++) {
	  wBreaksToChange = adjustBreaks(*p, wBreaksToChange, wGrids, wShiftLeads,
	wTrainees);
	}
	wShiftLeads = backTrack(wShiftLeads, wBreaksToChange);
	 for (auto p = wTrainees.begin(); p != wTrainees.end(); p++) {
	  wBreaksToChange = adjustBreaks(*p, wBreaksToChange, wGrids, wShiftLeads,
	wTrainees);
	}
	wTrainees = backTrack(wTrainees, wBreaksToChange);
	*/
	b << "Wednesday (Weekending " << filename << ")" << endl;
	b << "***************************************************************"
	  << endl
	  << "Cashier Trained Associates" << endl;
	b << "***************************************************************"
	  << endl
	  << endl;
	for (auto it = wAssociates.cbegin(); it != wAssociates.cend(); it++) {
		b << *it << endl;
	}
	b << endl
	  << "***************************************************************"
	  << endl;
	b << "***************************************************************"
	  << endl
	  << "Baggers/Service Clerks" << endl;
	b << "***************************************************************"
	  << endl
	  << endl;
	for (auto it = wBaggers.cbegin(); it != wBaggers.cend(); it++) {
		b << *it << endl;
	}
	b << endl
	  << "***************************************************************"
	  << endl;
	b << "***************************************************************"
	  << endl
	  << "Service Leaders" << endl;
	b << "***************************************************************"
	  << endl
	  << endl;
	for (auto it = wShiftLeads.cbegin(); it != wShiftLeads.cend(); it++) {
		b << *it << endl;
	}
	b << endl
	  << "***************************************************************"
	  << endl;
	b << "***************************************************************"
	  << endl
	  << "Training" << endl;
	b << "***************************************************************"
	  << endl
	  << endl;
	for (auto it = wTrainees.cbegin(); it != wTrainees.cend(); it++) {
		b << *it << endl;
	}
	b << endl
	  << "***************************************************************"
	  << endl;
	b.close();
	b.open("w-e" + filename + "/" + filename + "-5thursgrids.txt");
	thAssociates.sort(compareStartTimes);
	thBaggers.sort(compareStartTimes);
	thShiftLeads.sort(compareStartTimes);
	thTrainees.sort(compareStartTimes);
	thAssociates = backTrack(thAssociates, thBreaksToChange);
	thBaggers = backTrack(thBaggers, thBreaksToChange);
	thShiftLeads = backTrack(thShiftLeads, thBreaksToChange);
	thTrainees = backTrack(thTrainees, thBreaksToChange);
	/*thBreaksToChange.clear();
	for (auto p = thAssociates.begin(); p != thAssociates.end(); p++) {
	  thBreaksToChange = adjustBreaks(*p, thBreaksToChange, thGrids,
	thShiftLeads, thTrainees);
	}
	thAssociates = backTrack(thAssociates, thBreaksToChange);
	 for (auto p = thShiftLeads.begin(); p != thShiftLeads.end(); p++) {
	  thBreaksToChange = adjustBreaks(*p, thBreaksToChange, thGrids,
	thShiftLeads, thTrainees);
	}
	thShiftLeads = backTrack(thShiftLeads, thBreaksToChange);
	 for (auto p = thTrainees.begin(); p != thTrainees.end(); p++) {
	  thBreaksToChange = adjustBreaks(*p, thBreaksToChange, thGrids,
	thShiftLeads, thTrainees);
	}
	thTrainees = backTrack(thTrainees, thBreaksToChange);
	*/
	b << "Thursday (Weekending " << filename << ")" << endl;
	b << "***************************************************************"
	  << endl
	  << "Cashier Trained Associates" << endl;
	b << "***************************************************************"
	  << endl
	  << endl;
	for (auto it = thAssociates.cbegin(); it != thAssociates.cend(); it++) {
		b << *it << endl;
	}
	b << endl
	  << "***************************************************************"
	  << endl;
	b << "***************************************************************"
	  << endl
	  << "Baggers/Service Clerks" << endl;
	b << "***************************************************************"
	  << endl
	  << endl;
	for (auto it = thBaggers.cbegin(); it != thBaggers.cend(); it++) {
		b << *it << endl;
	}
	b << endl
	  << "***************************************************************"
	  << endl;
	b << "***************************************************************"
	  << endl
	  << "Service Leaders" << endl;
	b << "***************************************************************"
	  << endl
	  << endl;
	for (auto it = thShiftLeads.cbegin(); it != thShiftLeads.cend(); it++) {
		b << *it << endl;
	}
	b << endl
	  << "***************************************************************"
	  << endl;
	b << "***************************************************************"
	  << endl
	  << "Training" << endl;
	b << "***************************************************************"
	  << endl
	  << endl;
	for (auto it = thTrainees.cbegin(); it != thTrainees.cend(); it++) {
		b << *it << endl;
	}
	b << endl
	  << "***************************************************************"
	  << endl;
	b.close();
	b.open("w-e" + filename + "/" + filename + "-6frigrids.txt");
	fAssociates.sort(compareStartTimes);
	fBaggers.sort(compareStartTimes);
	fShiftLeads.sort(compareStartTimes);
	fTrainees.sort(compareStartTimes);
	fAssociates = backTrack(fAssociates, fBreaksToChange);
	fBaggers = backTrack(fBaggers, fBreaksToChange);
	fShiftLeads = backTrack(fShiftLeads, fBreaksToChange);
	fTrainees = backTrack(fTrainees, fBreaksToChange);
	/*
	fBreaksToChange.clear();
	for (auto p = fAssociates.begin(); p != fAssociates.end(); p++) {
	  fBreaksToChange = adjustBreaks(*p, fBreaksToChange, fGrids, fShiftLeads,
	fTrainees);
	}
	fAssociates = backTrack(fAssociates, fBreaksToChange);
	 for (auto p = fShiftLeads.begin(); p != fShiftLeads.end(); p++) {
	  fBreaksToChange = adjustBreaks(*p, fBreaksToChange, fGrids, fShiftLeads,
	fTrainees);
	}
	fShiftLeads = backTrack(fShiftLeads, fBreaksToChange);
	 for (auto p = fTrainees.begin(); p != fTrainees.end(); p++) {
	  fBreaksToChange = adjustBreaks(*p, fBreaksToChange, fGrids, fShiftLeads,
	fTrainees);
	}
	fTrainees = backTrack(fTrainees, fBreaksToChange);
	*/
	b << "Friday (Weekending " << filename << ")" << endl;
	b << "***************************************************************"
	  << endl
	  << "Cashier Trained Associates" << endl;
	b << "***************************************************************"
	  << endl
	  << endl;
	for (auto it = fAssociates.cbegin(); it != fAssociates.cend(); it++) {
		b << *it << endl;
	}
	b << endl
	  << "***************************************************************"
	  << endl;
	b << "***************************************************************"
	  << endl
	  << "Baggers/Service Clerks" << endl;
	b << "***************************************************************"
	  << endl
	  << endl;
	for (auto it = fBaggers.cbegin(); it != fBaggers.cend(); it++) {
		b << *it << endl;
	}
	b << endl
	  << "***************************************************************"
	  << endl;
	b << "***************************************************************"
	  << endl
	  << "Service Leaders" << endl;
	b << "***************************************************************"
	  << endl
	  << endl;
	for (auto it = fShiftLeads.cbegin(); it != fShiftLeads.cend(); it++) {
		b << *it << endl;
	}
	b << endl
	  << "***************************************************************"
	  << endl;
	b << "***************************************************************"
	  << endl
	  << "Training" << endl;
	b << "***************************************************************"
	  << endl
	  << endl;
	for (auto it = fTrainees.cbegin(); it != fTrainees.cend(); it++) {
		b << *it << endl;
	}
	b << endl
	  << "***************************************************************"
	  << endl;
	b.close();
	b.open("w-e" + filename + "/" + filename + "-7satgrids.txt");
	saAssociates.sort(compareStartTimes);
	saBaggers.sort(compareStartTimes);
	saShiftLeads.sort(compareStartTimes);
	saTrainees.sort(compareStartTimes);
	saAssociates = backTrack(saAssociates, saBreaksToChange);
	saBaggers = backTrack(saBaggers, saBreaksToChange);
	saShiftLeads = backTrack(saShiftLeads, saBreaksToChange);
	saTrainees = backTrack(saTrainees, saBreaksToChange);
	/*
	saBreaksToChange.clear();
	for (auto p = saAssociates.begin(); p != saAssociates.end(); p++) {
	  saBreaksToChange = adjustBreaks(*p, saBreaksToChange, saGrids,
	saShiftLeads, saTrainees);
	}
	saAssociates = backTrack(saAssociates, saBreaksToChange);
	 for (auto p = saShiftLeads.begin(); p != saShiftLeads.end(); p++) {
	  saBreaksToChange = adjustBreaks(*p, saBreaksToChange, saGrids,
	saShiftLeads, saTrainees);
	}
	saShiftLeads = backTrack(saShiftLeads, saBreaksToChange);
	 for (auto p = saTrainees.begin(); p != saTrainees.end(); p++) {
	  saBreaksToChange = adjustBreaks(*p, saBreaksToChange, saGrids,
	saShiftLeads, saTrainees);
	}
	saTrainees = backTrack(saTrainees, saBreaksToChange);
	*/
	b << "Saturday (Weekending " << filename << ")" << endl;
	b << "***************************************************************"
	  << endl
	  << "Cashier Trained Associates" << endl;
	b << "***************************************************************"
	  << endl
	  << endl;
	for (auto it = saAssociates.cbegin(); it != saAssociates.cend(); it++) {
		b << *it << endl;
	}
	b << endl
	  << "***************************************************************"
	  << endl;
	b << "***************************************************************"
	  << endl
	  << "Baggers/Service Clerks" << endl;
	b << "***************************************************************"
	  << endl
	  << endl;
	for (auto it = saBaggers.cbegin(); it != saBaggers.cend(); it++) {
		b << *it << endl;
	}
	b << endl
	  << "***************************************************************"
	  << endl;
	b << "***************************************************************"
	  << endl
	  << "Service Leaders" << endl;
	b << "***************************************************************"
	  << endl
	  << endl;
	for (auto it = saShiftLeads.cbegin(); it != saShiftLeads.cend(); it++) {
		b << *it << endl;
	}
	b << endl
	  << "***************************************************************"
	  << endl;
	b << "***************************************************************"
	  << endl
	  << "Training" << endl;
	b << "***************************************************************"
	  << endl
	  << endl;
	for (auto it = saTrainees.cbegin(); it != saTrainees.cend(); it++) {
		b << *it << endl;
	}
	b << endl
	  << "***************************************************************"
	  << endl;
	b.close();

	return 0;
}
