/*
 * CSVHandler.h
 *
 *  Created on: Jul 17, 2019
 *      Author: hsenid
 */

#ifndef LIBRARY_MANAGEMENT_SYSTEM_BACKEND_LIBRARYSERVER_SRC_REPORTING_CSVHANDLER_H_
#define LIBRARY_MANAGEMENT_SYSTEM_BACKEND_LIBRARYSERVER_SRC_REPORTING_CSVHANDLER_H_

#include <iostream>
#include <vector>
using namespace std;

class CSVHandler
{
public:
	CSVHandler(){}
	~CSVHandler(){}
	void writeToCSV(vector<string> rows, string header, string path);
};



#endif /* LIBRARY_MANAGEMENT_SYSTEM_BACKEND_LIBRARYSERVER_SRC_REPORTING_CSVHANDLER_H_ */
