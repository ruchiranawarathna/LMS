#include <iostream>
#include <ctime>
#include <sstream>

#include "DateUtils.h"
#include "../ConfigHandler.h"

using namespace std;

string DateUtils::m_today;
string DateUtils::m_dueDate;

const time_t ONE_DAY = 24 * 60 * 60;
const int SECONDS_PER_DAY = 60*60*24;
int BASE_YEAR;

std::tm DateUtils::make_tm(int year, int month, int day)
{
    std::tm tm = {0};
    tm.tm_year = year - BASE_YEAR; // years count from 1900
    tm.tm_mon = month - 1;    // months count from January=0
    tm.tm_mday = day;         // days count from 1
    return tm;
}

int DateUtils::getDateDifference(string date1, string date2)
{
	cout << "DateUtils::getDateDifference [" + date1 + "] [" + date2 + "]" << endl;
	try
	{
		int year1 = std::stoi(date1.substr(0,4));
		int month1 = std::stoi(date1.substr(5,2));
		int day1 = std::stoi(date1.substr(8,2));
		int year2 = std::stoi(date2.substr(0,4));
		int month2 = std::stoi(date2.substr(5,2));
		int day2 = std::stoi(date2.substr(8,2));

		std::tm tm1 = DateUtils::make_tm(year1,month1,day1);
		std::tm tm2 = DateUtils::make_tm(year2,month2,day2);

		// Arithmetic time values.
		// On a posix system, these are seconds since 1970-01-01 00:00:00 UTC
		std::time_t time1 = std::mktime(&tm1);
		std::time_t time2 = std::mktime(&tm2);

		// Divide by the number of seconds in a day
		std::time_t difference = (time2 - time1) / SECONDS_PER_DAY;

		// To be fully portable, we shouldn't assume that these are Unix time;
		// instead, we should use "difftime" to give the difference in seconds:
		return std::difftime(time2, time1) / SECONDS_PER_DAY;
	}
	catch(const std::invalid_argument& ia)
	{
		cout << "\nInvalid input date difference" << endl;
	}
	return -1;
}

void DateUtils::setToday()
{
	cout << "DateUtils::setToday" << endl;
	time_t t = time(0);   // get time now
	struct tm * now = localtime( & t );
	std::stringstream  today;

	string year = std::to_string(now->tm_year + BASE_YEAR);
	string month = std::to_string(now->tm_mon + 1);
	string date = std::to_string(now->tm_mday);

	if((now->tm_mon + 1) < 10)
		month = "0" + month;
	if((now->tm_mday) < 10)
		date = "0" + date;

	today << year << '-'	<< month << '-'	<<  date;

	m_today = today.str();
}

void DateUtils::setDueDate(int issuePeriod)
{
	cout << "DateUtils::setDueDate:" << issuePeriod << endl;
	time_t t = time(0);   // get time now
	struct tm * now = localtime( & t );
    // Seconds since start of epoch
    time_t date_seconds = mktime( now ) + (issuePeriod * ONE_DAY) ;
    struct tm * due = localtime( & date_seconds );

	std::stringstream  dueDate;

	string year = std::to_string(due->tm_year + BASE_YEAR);
	string month = std::to_string(due->tm_mon + 1);
	string date = std::to_string(due->tm_mday);

	if((now->tm_mon + 1) < 10)
		month = "0" + month;
	if((now->tm_mday) < 10)
		date = "0" + date;

	dueDate << year << '-'	<< month << '-'	<<  date;

	m_dueDate = dueDate.str();
}

void DateUtils::init()
{
	BASE_YEAR = atoi(ConfigHandler::getConfigValue("BASE_YEAR").c_str());
	DateUtils::setToday();
}


