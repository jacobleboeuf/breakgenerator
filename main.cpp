#include <bits/stdc++.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <string>
using namespace std;
class breakTimes {
public:
	breakTimes(string iName, string iStartTime, string iEndTime) {
		name = iName;
		startTime = iStartTime;
		endTime = iEndTime;
		findBreaks();
    adjustBreaks();
	}
	breakTimes(
		string iName,
		string iStartTime,
		string iEndTime,
		bool iIsAdult) {
		name = iName;
		startTime = iStartTime;
		endTime = iEndTime;
		isAdult = iIsAdult;
		findBreaks();
    //adjustBreaks();
	}
	breakTimes(
		string iName,
		string iStartTime,
		string iEndTime,
		bool iIsAdult,
		bool iIsTraining) {
		name = iName;
		startTime = iStartTime;
		endTime = iEndTime;
		isAdult = iIsAdult;
		isTraining = iIsTraining;
		findBreaks();
   // adjustBreaks();
	}
	breakTimes(
		string iName,
		string iStartTime,
		string iEndTime,
		bool iIsAdult,
		bool iIsShiftLead,
		bool iIsTraining) {
		name = iName;
		startTime = iStartTime;
		endTime = iEndTime;
		isAdult = iIsAdult;
		isShiftLead = iIsShiftLead;
		isTraining = iIsTraining;
		findBreaks();
   // adjustBreaks();
	}
	string name, startTime, endTime;
	int totalHrs, totalMins;
	list<string> breaks;
	bool isShiftLead, isAdult, isTraining;
	void calcTotalShiftTime();
	void findBreaks();
  void adjustBreaks();
	string checkBreak(string newBreak);
	friend ostream &operator<<(ostream &os, const breakTimes &b);
};
ofstream b;
list<breakTimes> grids;
list<breakTimes> shiftLeads;
list<breakTimes> associates;
list<breakTimes> trainees;
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
// remove ':' and convert it into an integer
int removeColon(string s) {
	if (s.size() == 4)
		s.replace(1, 1, "");

	if (s.size() == 5)
		s.replace(2, 1, "");

	return stoi(s);
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
string breakTimes::checkBreak(string newBreak) {
	string s = newBreak;
	// cout << newBreak << endl;
	if (name == "Anita") {
		b << "AAAAAAAAA" << endl;
	}
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
					}
				}
			}
		}
		return newBreak;
	}
	while (breakChecked == false) {
		for (auto it = grids.begin(); it != grids.end(); it++) {
			for (auto p = (*it).breaks.begin(); p != (*it).breaks.end(); p++) {
				if (newBreak == *p) {
					if ((endTime != "10:00" && (*it).endTime != "10:00") ||
						(endTime == "10:00" && (*it).endTime == "10:00") ||
						(endTime == "10:00" &&
						 ((*it).endTime != "10:30" &&
						  (*it).endTime != "3:00")) ||
						((*it).endTime == "10:00" && endTime != "10:30" &&
						 endTime != "3:00")) {
						for (auto q = grids.cbegin(); q != grids.cend(); q++) {
							for (auto g = (*q).breaks.begin();
								 g != (*q).breaks.end();
								 g++) {
								if (*g == addMin(s, 15)) {
									bft = true;
								}
								if (*g == addMin(s, -15)) {
									bnft = true;
								}
								if (*g == addMin(s, 30)) {
									bthrt = true;
								}
							}
						}
						if (!bft) {
							if (diff(addMin(newBreak, 15), startTime) < 3) {
							  newBreak = addMin(newBreak, 15);
              }
						} else if (!bnft) {
							newBreak = addMin(newBreak, -15);
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
	return newBreak;
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
		(totalHrs == 3 && totalMins < 30)) {
		// Shift doesn't need break
		return;
	} else if (totalHrs < 6) {
		// Shift with only 1 break
		breakHr = removeColon(startTime) / 100 + (totalHrs / 2);
		if ((totalMins >= 30) && (totalHrs == 5)) {
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
		newBreak = checkBreak(newBreak);
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
			newBreak = checkBreak(newBreak);
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
			newBreak = checkBreak(newBreak);
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
			newBreak = checkBreak(newBreak);
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
			newBreak = checkBreak(newBreak);
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
void breakTimes::adjustBreaks() {
  for (list<breakTimes>::iterator p = grids.begin(); p != grids.end(); p++) {
    for (auto q = (*p).breaks.begin(); q != (*p).breaks.end(); q++) {
      *q = checkBreak(*q);
    }
  }
}
// Driver program to test above functions
int main() {
	string input, sTime, eTime, filename;
	char a;
	bool adult = false, training = false, shiftLead = false;
	cout << endl << "Enter day of grids (i.e. March14): ";
	cin >> filename;
	b.open(filename + "grids.txt");
	while (input != "Q" && input != "q") {
		cout << endl << "Enter associate name or enter Q to quit: ";
		cin >> input;
		if (input != "q" && input != "Q") {
			cout << "Enter shift start time, with a colon (ex: 7:30): ";
			cin >> sTime;
			cout << endl << "Enter shift end time, with a colon (ex: 3:30): ";
			cin >> eTime;
			cout << endl << "Are they over 18? (Y or N): ";
			cin >> a;
			if (a == 'Y' || a == 'y') {
				adult = true;
			}
			cout << endl << "Are they a shift leader? (Y or N): ";
			cin >> a;
			if (a == 'Y' || a == 'y') {
				shiftLead = true;
			}
			cout << "Are they training? (Y or N): ";
			cin >> a;
			if (a == 'Y' || a == 'y') {
				training = true;
			}

			breakTimes t(input, sTime, eTime, adult, shiftLead, training);
			if (training) {
				trainees.push_back(t);
			} else if (shiftLead) {
				shiftLeads.push_back(t);
			} else {
				associates.push_back(t);
			}
			grids.push_back(t);
			adult = false;
			training = false;
			shiftLead = false;
		}
	}
	b << "***************************************************************"
	  << endl
	  << "Cashier/Service Clerk" << endl;
	b << "***************************************************************"
	  << endl
	  << endl;
	for (auto it = associates.cbegin(); it != associates.cend(); it++) {
		b << *it << endl;
	}
	b << endl
	  << "***************************************************************"
	  << endl;
	// printGrids(associates, b);
	b << "***************************************************************"
	  << endl
	  << "Service Leaders" << endl;
	b << "***************************************************************"
	  << endl
	  << endl;
	for (auto it = shiftLeads.cbegin(); it != shiftLeads.cend(); it++) {
		b << *it << endl;
	}
	b << endl
	  << "***************************************************************"
	  << endl;
	// printGrids(shiftLeads, b);
	b << "***************************************************************"
	  << endl
	  << "Training" << endl;
	b << "***************************************************************"
	  << endl
	  << endl;
	for (auto it = trainees.cbegin(); it != trainees.cend(); it++) {
		b << *it << endl;
	}
	b << endl
	  << "***************************************************************"
	  << endl;
	// printGrids(trainees, b);
	return 0;
}
