/*
 * ConfigHandler.h
 *
 *  Created on: Jul 17, 2019
 *      Author: hsenid
 */

#ifndef LIBRARY_MANAGEMENT_SYSTEM_BACKEND_LIBRARYSERVER_SRC_CONFIGHANDLER_H_
#define LIBRARY_MANAGEMENT_SYSTEM_BACKEND_LIBRARYSERVER_SRC_CONFIGHANDLER_H_

#include <iostream>
#include <map>

using namespace std;

class ConfigHandler
{
public:
	static bool init(string path);
	static string getConfigValue(string key);
private:
	static map<string, string> m_configMap;
};



#endif /* LIBRARY_MANAGEMENT_SYSTEM_BACKEND_LIBRARYSERVER_SRC_CONFIGHANDLER_H_ */
