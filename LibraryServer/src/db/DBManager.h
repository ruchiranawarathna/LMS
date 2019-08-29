#ifndef LIBRARY_MANAGEMENT_SYSTEM_BACKEND_LIBRARYSERVER_SRC_DB_DBMANAGER_H_
#define LIBRARY_MANAGEMENT_SYSTEM_BACKEND_LIBRARYSERVER_SRC_DB_DBMANAGER_H_

#include <iostream>
#include <cpprest/json.h>
#include <vector>
using namespace std;

using namespace web;

class DBManager {
public:
	DBManager(){}
	~DBManager(){}

	int addBook(vector<pair<string, string>> kvPairs);
	int addUser(vector<pair<string, string>> kvPairs);
	int addLend(vector<pair<string, string>> kvPairs);
	int removeBook(vector<pair<string, string>> kvPairs);
	int removeUser(vector<pair<string, string>> kvPairs);
	int updateBook(vector<pair<string, string>> kvPairs);
	int updateUser(vector<pair<string, string>> kvPairs);
	int updateLend(vector<pair<string, string>> kvPairs);
	int updateConfigs(vector<pair<string, string>> kvPairs);
	int getTotalLendCount();
	json::value getAllBooks(vector<pair<string, string>> kvPairs);
	json::value getAllUsers(vector<pair<string, string>> kvPairs);
	json::value getCurrentLends(vector<pair<string, string>> kvPairs);
	string getConfigValue(string libraryId, string key);
	json::value getUser(string userId);
	bool isValidUser(string userID);
	bool isValidBook(string bookCode);
	bool isBookAvailable(string bookCode);
	void updateDelays(vector<pair<string, string>> kvPairs);
	void notify(json::value lend, int delay);

private:
};



#endif /* LIBRARY_MANAGEMENT_SYSTEM_BACKEND_LIBRARYSERVER_SRC_DB_DBMANAGER_H_ */
