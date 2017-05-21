#pragma once

#include <string>

class Date {
private:
	enum MONTHS { JANUARY = 1, FEBRUARY, MARCH, APRIL, MAY, JUNE, JULY, AUGUST, SEPTEMBER, OCTOBER, NOVEMBER, DECEMBER };
	static int second, minute, hour, day, month, year;
	static unsigned long long lastT;
	static void UpdateMonth(unsigned long long &t, int monthDays);
public:
	static std::string SecondsToDate(unsigned long long t);
};
