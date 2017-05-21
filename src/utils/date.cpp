#include "utils/date.h"

unsigned long long Date::lastT = 0LL;
int Date::second = 0;
int Date::minute = 0;
int Date::hour = 0;
int Date::day = 1;
int Date::month = 1;
int Date::year = 2015;

void Date::UpdateMonth(unsigned long long &t, int monthDays) {
	if (t >= (unsigned long long)(monthDays - Date::day + 1)) {
		t = t - (monthDays - Date::day + 1);
		Date::day = 1;
		++Date::month;
		if (Date::month == 13) {
			Date::month = 1;
			++Date::year;
		}
	}
	else {
		Date::day += (int)t;
		t = 0;
	}
}

std::string Date::SecondsToDate(unsigned long long t) {
	std::string date;

	if (lastT != t) {
		unsigned long long prevT = t;
		t = t - lastT;
		lastT = prevT;

		Date::second = (t = Date::second + t) % 60;
		t /= 60;

		Date::minute = (t = Date::minute + t) % 60;
		t /= 60;

		Date::hour = (t = Date::hour + t) % 24;
		t /= 24;

		while (t > 0) {
			if (Date::month == Date::MONTHS::FEBRUARY) {
				bool leapYear = Date::year % 4 == 0 && (Date::year % 100 != 0 || Date::year % 400 == 0) ? true : false;
				if (leapYear) {
					Date::UpdateMonth(t, 29);
				}
				else {
					Date::UpdateMonth(t, 28);
				}
			}
			else if (Date::month == Date::MONTHS::JANUARY ||
				Date::month == Date::MONTHS::MARCH ||
				Date::month == Date::MONTHS::MAY ||
				Date::month == Date::MONTHS::JULY ||
				Date::month == Date::MONTHS::AUGUST ||
				Date::month == Date::MONTHS::OCTOBER ||
				Date::month == Date::MONTHS::DECEMBER) {
				Date::UpdateMonth(t, 31);
			}
			else if (Date::month == Date::MONTHS::APRIL ||
				Date::month == Date::MONTHS::JUNE ||
				Date::month == Date::MONTHS::SEPTEMBER ||
				Date::month == Date::MONTHS::NOVEMBER) {
				Date::UpdateMonth(t, 30);
			}
		}
	}

	if (Date::day < 10) {
		date = "0";
	}
	date += std::to_string(Date::day) + "/";
	if (Date::month < 10) {
		date += "0";
	}
	date += std::to_string(Date::month) + "/" +
		std::to_string(Date::year) + " - ";
	if (Date::hour < 10) {
		date += "0";
	}
	date += std::to_string(Date::hour) + ":";
	if (Date::minute < 10) {
		date += "0";
	}
	date += std::to_string(Date::minute) + ":";
	if (Date::second < 10) {
		date += "0";
	}
	date += std::to_string(Date::second);

	return date;
}
