#ifndef LIBRARY_MANAGEMENT_SYSTEM_BACKEND_LIBRARYSERVER_SRC_UTILS_DATEUTILS_H_
#define LIBRARY_MANAGEMENT_SYSTEM_BACKEND_LIBRARYSERVER_SRC_UTILS_DATEUTILS_H_

#include <iostream>

using namespace std;
class DateUtils
{
public:
	static string getCurrentDate() {return m_today;}
	static string getDueDate() {return m_dueDate;}
	static int getDateDifference(string date1, string date2);
	static void setDueDate(int issuePeriod);
	static void init();

private:
	static void setToday();
	static tm make_tm(int year, int month, int day);
	static string m_today;
	static string m_dueDate;
};


#endif /* LIBRARY_MANAGEMENT_SYSTEM_BACKEND_LIBRARYSERVER_SRC_UTILS_DATEUTILS_H_ */
