#include "DBManager.h"
#include <iostream>
#include <map>
#include "../Errors.h"
#include "../utils/DateUtils.h"
#include "../ConfigHandler.h"
#include "../notify/SMSNotifier.h"

#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/builder/stream/document.hpp>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;
using namespace std;

map<string, string> bookFields = {{"libraryId", "libraryId"}, {"code", "code"}, {"title", "title"}, {"author", "author"}, {"category", "category"}};
map<string, string> userFields = {{"libraryId", "libraryId"}, {"userId", "userId"}, {"name", "name"}, {"contact", "contact"}, {"address", "address"}};
map<string, string> lendFields = {{"libraryId", "libraryId"}, {"lendId", "lendId"}, {"userId", "userId"}, {"bookCode", "bookCode"}, {"issueDate", "issueDate"},
			{"dueDate", "dueDate"}, {"delay", "delay"}, {"fine", "fine"}, {"isReturned", "isReturned"}};
map<string, string> configFields = {{"libraryId", "libraryId"}, {"finePerDay", "finePerDay"}, {"issuePeriod", "issuePeriod"}};

int DBManager::addBook(vector<pair<string, string>> kvPairs)
{
	cout << "DBManager::addBook" << endl;
	if(kvPairs.size() > 0)
	{
		mongocxx::client client{mongocxx::uri{}};
		auto collection = client["library"]["books"];
		bsoncxx::builder::stream::document document{};

		for(pair<string, string> kv : kvPairs)
		{
			cout << "Pair=>" << kv.first << ":" << kv.second << endl;
			document << kv.first << kv.second;
			if(kv.first == bookFields["code"])
			{
				cout << "Checking books for code:" << kv.second << endl;
			    bsoncxx::stdx::optional<bsoncxx::document::value> result =
			      collection.find_one(bsoncxx::builder::stream::document{} << bookFields["code"] << kv.second << finalize);
				if(result) {
				  cout << "Book already exist..." << endl;
				  return Errors::DUPLICATE_BOOK_CODE;

				}
				document << "_id" << kv.second;
			}
		}
		collection.insert_one(document.view());
	}
	return 0;
}

int DBManager::addUser(vector<pair<string, string>> kvPairs)
{
	cout << "DBManager::addUser" << endl;
	if(kvPairs.size() > 0)
	{
		mongocxx::client client{mongocxx::uri{}};
		auto collection = client["library"]["users"];
		bsoncxx::builder::stream::document document{};

		for(pair<string, string> kv : kvPairs)
		{
			cout << "Pair=>" << kv.first << ":" << kv.second << endl;
			document << kv.first << kv.second;
			if(kv.first == userFields["userId"])
			{
				cout << "Checking users for id:" << kv.second << endl;
			    bsoncxx::stdx::optional<bsoncxx::document::value> result =
			      collection.find_one(bsoncxx::builder::stream::document{} << userFields["userId"] << kv.second << finalize);
				if(result) {
				  cout << "User already exist..." << endl;
				  return Errors::DUPLICATE_USER_ID;
				}
				document << "_id" << kv.second;
			}
		}
		collection.insert_one(document.view());
	}
	return 0;
}

int DBManager::addLend(vector<pair<string, string>> kvPairs)
{
	cout << "DBManager::addLend" << endl;
	if(kvPairs.size() > 0)
	{
		mongocxx::client client{mongocxx::uri{}};
		auto collection = client["library"]["lends"];
		bsoncxx::builder::stream::document document{};
		mongocxx::cursor cursor = collection.find({});
		try
		{
			DateUtils::setDueDate(stoi(getConfigValue("testlibrary", configFields["issuePeriod"])));
		}
		catch(const std::invalid_argument& ia)
		{
			cout << "\nInvalid input for issue period" << endl;
		}

		int records = getTotalLendCount();
		cout << "record count:" << records << endl;
//		document << "_id" << to_string(records+1);
		document << lendFields["lendId"] << to_string(records+1);
		document << lendFields["issueDate"] << DateUtils::getCurrentDate();
		document << lendFields["dueDate"] << DateUtils::getDueDate();
		document << lendFields["delay"] << to_string(0);
		document << lendFields["fine"] << to_string(0);
		document << lendFields["isReturned"] << to_string(0);

		for(pair<string, string> kv : kvPairs)
		{
			cout << "Pair=>" << kv.first << ":" << kv.second << endl;
			if(kv.first == lendFields["libraryId"])
			{
				document << kv.first << kv.second;
				document << "_id" << kv.second + to_string(records+1);
			}
			if(kv.first == lendFields["userId"])
			{
				if(!isValidUser(kv.second))
				{
					return Errors::INVALID_USER_ID;
				}
				document << kv.first << kv.second;
			}
			else if(kv.first == lendFields["bookCode"])
			{
				if(!isValidBook(kv.second))
				{
					return Errors::INVALID_BOOK_CODE;
				}
				if(!isBookAvailable(kv.second))
				{
					return Errors::BOOK_NOT_AVAILABLE;
				}
				document << kv.first << kv.second;
			}
		}
		collection.insert_one(document.view());
	}
	return 0;
}

int DBManager::removeBook(vector<pair<string, string>> kvPairs)
{
	cout << "DBManager::removeBook" << endl;
	if(kvPairs.size() > 0)
	{
		string code;
		mongocxx::client client{mongocxx::uri{}};
		auto collection = client["library"]["books"];

		for(pair<string, string> kv : kvPairs)
		{
			cout << "Pair=>" << kv.first << ":" << kv.second << endl;
			if(kv.first == bookFields["code"])
				code = kv.second;
		}
		collection.delete_one(bsoncxx::builder::stream::document{} << bookFields["code"] << code << finalize);
	}
	return 0;
}

int DBManager::removeUser(vector<pair<string, string>> kvPairs)
{
	cout << "DBManager::removeUser" << endl;
	if(kvPairs.size() > 0)
	{
		string userId;
		mongocxx::client client{mongocxx::uri{}};
		auto collection = client["library"]["users"];

		for(pair<string, string> kv : kvPairs)
		{
			cout << "Pair=>" << kv.first << ":" << kv.second << endl;
			if(kv.first == userFields["userId"])
				userId = kv.second;
		}
		collection.delete_one(bsoncxx::builder::stream::document{} << userFields["userId"] << userId << finalize);
	}
	return 0;
}

int DBManager::updateBook(vector<pair<string, string>> kvPairs)
{
	cout << "DBManager::updateBook" << endl;
	if(kvPairs.size() > 0)
	{
		string code;
		string title;
		string author;
		string category;

		mongocxx::client client{mongocxx::uri{}};
		auto collection = client["library"]["books"];

		for(pair<string, string> kv : kvPairs)
		{
			cout << "Pair=>" << kv.first << ":" << kv.second << endl;
			if(kv.first == bookFields["code"])
				code = kv.second;
			if(kv.first == bookFields["title"])
				title = kv.second;
			if(kv.first == bookFields["author"])
				author = kv.second;
			if(kv.first == bookFields["category"])
				category = kv.second;
		}

		collection.update_one(bsoncxx::builder::stream::document{}	<< bookFields["code"] << code << finalize,
				bsoncxx::builder::stream::document{} << "$set" << open_document
				<< bookFields["code"] << code
				<< bookFields["title"] << title
				<< bookFields["author"] << author
				<< bookFields["category"] << category << close_document << finalize);
	}
	return 0;
}

int DBManager::updateUser(vector<pair<string, string>> kvPairs)
{
	cout << "DBManager::updateUser" << endl;
	if(kvPairs.size() > 0)
	{
		string userId;
		string name;
		string contact;
		string address;

		mongocxx::client client{mongocxx::uri{}};
		auto collection = client["library"]["users"];

		for(pair<string, string> kv : kvPairs)
		{
			cout << "Pair=>" << kv.first << ":" << kv.second << endl;
			if(kv.first == userFields["userId"])
				userId = kv.second;
			if(kv.first == userFields["name"])
				name = kv.second;
			if(kv.first == userFields["contact"])
				contact = kv.second;
			if(kv.first == userFields["address"])
				address = kv.second;
		}

		collection.update_one(bsoncxx::builder::stream::document{}	<< userFields["userId"] << userId << finalize,
				bsoncxx::builder::stream::document{} << "$set" << open_document
				<< userFields["userId"] << userId
				<< userFields["name"] << name
				<< userFields["contact"] << contact
				<< userFields["address"] << address << close_document << finalize);
	}
	return 0;
}


int DBManager::updateConfigs(vector<pair<string, string>> kvPairs)
{
	cout << "DBManager::updateConfigs" << endl;
	if(kvPairs.size() > 0)
	{
		string libraryId;
		string finePerDay;
		string issuePeriod;

		mongocxx::client client{mongocxx::uri{}};
		auto collection = client["library"]["configs"];

		for(pair<string, string> kv : kvPairs)
		{
			cout << "Pair=>" << kv.first << ":" << kv.second << endl;
			if(kv.first == configFields["libraryId"])
				libraryId = kv.second;
			if(kv.first == configFields["finePerDay"])
				finePerDay = kv.second;
			if(kv.first == configFields["issuePeriod"])
				issuePeriod = kv.second;
		}

		bsoncxx::stdx::optional<bsoncxx::document::value> result =
				collection.find_one(bsoncxx::builder::stream::document{} << configFields["libraryId"] << libraryId << finalize);
		if(!result) {
			cout << "Config for library not found. Create one. " << endl;
			bsoncxx::builder::stream::document document{};

			for(pair<string, string> kv : kvPairs)
			{
				cout << "Pair=>" << kv.first << ":" << kv.second << endl;
				document << kv.first << kv.second;
			}
			collection.insert_one(document.view());
		}
		else
		{
			collection.update_one(bsoncxx::builder::stream::document{}	<< configFields["libraryId"] << libraryId << finalize,
				bsoncxx::builder::stream::document{} << "$set" << open_document
				<< configFields["finePerDay"] << finePerDay
				<< configFields["issuePeriod"] << issuePeriod << close_document << finalize);
		}
	}
	return 0;
}

int DBManager::updateLend(vector<pair<string, string>> kvPairs)
{
	cout << "DBManager::updateLend" << endl;
	if(kvPairs.size() > 0)
	{
		string userId;
		string bookCode;
		string lendId;
		string isReturned;

		mongocxx::client client{mongocxx::uri{}};
		auto collection = client["library"]["lends"];

		for(pair<string, string> kv : kvPairs)
		{
			cout << "Pair=>" << kv.first << ":" << kv.second << endl;
			if(kv.first == lendFields["userId"])
			{
				if(!isValidUser(kv.second))
				{
					return Errors::INVALID_USER_ID;
				}
				userId = kv.second;
			}
			else if(kv.first == lendFields["bookCode"])
			{
				if(!isValidBook(kv.second))
				{
					return Errors::INVALID_BOOK_CODE;
				}
				bookCode = kv.second;
			}
			else if(kv.first == lendFields["lendId"])
				lendId = kv.second;
			else if(kv.first == lendFields["isReturned"])
				isReturned = kv.second;
		}

		if(!isBookAvailable(bookCode) && isReturned == "0")
		{
			return Errors::BOOK_NOT_AVAILABLE;
		}

		collection.update_one(bsoncxx::builder::stream::document{}	<< lendFields["lendId"] << lendId << finalize,
				bsoncxx::builder::stream::document{} << "$set" << open_document
				<< lendFields["userId"] << userId
				<< lendFields["bookCode"] << bookCode
				<< lendFields["lendId"] << lendId
				<< lendFields["isReturned"] << isReturned
				<< close_document << finalize);
	}
	return 0;
}

json::value DBManager::getAllBooks(vector<pair<string, string>> kvPairs)
{
	cout << "DBManager::getAllBooks" << endl;
	string libraryId = "";
	for(pair<string, string> kv : kvPairs)
	{
		if(kv.first == bookFields["libraryId"])
		{
			cout << "Get book for library:" << kv.second << endl;
		    libraryId = kv.second;
		}
	}
	json::value arr;
	mongocxx::client client{mongocxx::uri{}};
	auto collection = client["library"]["books"];

//	mongocxx::cursor cursor = collection.find({});
	mongocxx::cursor cursor = collection.find(document{} << bookFields["libraryId"] << open_document
			<< "$eq" << libraryId
			<< close_document << finalize);

	int i = 0;
	for(auto v : cursor) {
		json::value book;
		for (std::pair<std::string, string> field : bookFields)
		{
			bsoncxx::document::element eValue{v[field.first]};
			string value = eValue.get_utf8().value.to_string();
			book[U(field.first)] = json::value::string(U(value));
		}
		arr[i] = book;
		i++;
	}
	return arr;
}

json::value DBManager::getAllUsers(vector<pair<string, string>> kvPairs)
{
	cout << "DBManager::getAllUsers" << endl;
	string libraryId = "";
	for(pair<string, string> kv : kvPairs)
	{
		if(kv.first == userFields["libraryId"])
		{
			cout << "Get users for library:" << kv.second << endl;
		    libraryId = kv.second;
		}
	}
	json::value arr;
	mongocxx::client client{mongocxx::uri{}};
	auto collection = client["library"]["users"];
//	mongocxx::cursor cursor = collection.find({});
	mongocxx::cursor cursor = collection.find(document{} << userFields["libraryId"] << open_document
			<< "$eq" << libraryId
			<< close_document << finalize);

	int i = 0;
	for(auto v : cursor) {
		json::value user;
		for (std::pair<std::string, string> field : userFields)
		{
			bsoncxx::document::element eValue{v[field.first]};
			string value = eValue.get_utf8().value.to_string();
			user[U(field.first)] = json::value::string(U(value));
		}
		arr[i] = user;
		i++;
	}
	return arr;
}

json::value DBManager::getCurrentLends(vector<pair<string, string>> kvPairs)
{
	cout << "DBManager::getCurrentLends" << endl;
	string libraryId = "";
	for(pair<string, string> kv : kvPairs)
	{
		if(kv.first == lendFields["libraryId"])
		{
			cout << "Get lends for library:" << kv.second << endl;
		    libraryId = kv.second;
		}
	}
	json::value arr;
	mongocxx::client client{mongocxx::uri{}};
	auto collection = client["library"]["lends"];
	mongocxx::cursor cursor = collection.find(document{} << lendFields["libraryId"] << open_document
			<< "$eq" << libraryId
			<< close_document << finalize);
//	mongocxx::cursor cursor = collection.find(document{} << lendFields["isReturned"] << open_document
//			<< "$eq" << "0"
//			<< close_document << finalize);
	int i = 0;
	for(auto v : cursor) {
		json::value lend;
		for (std::pair<std::string, string> field : lendFields)
		{
			bsoncxx::document::element eValue{v[field.first]};
			string value = eValue.get_utf8().value.to_string();
			lend[U(field.first)] = json::value::string(U(value));
		}
		arr[i] = lend;
		i++;
	}
	return arr;
}

string DBManager::getConfigValue(string libraryId, string key)
{
	cout << "DBManager::getConfigValue:" + key << endl;
	mongocxx::client client{mongocxx::uri{}};
	auto collection = client["library"]["configs"];
	mongocxx::cursor cursor = collection.find(document{} << "libraryId" << open_document
			<< "$eq" << libraryId
			<< close_document << finalize);

	json::value arr;
	int i = 0;
	for(auto v : cursor) {
		json::value config;
		for (std::pair<std::string, string> field : configFields)
		{
			bsoncxx::document::element eValue{v[field.first]};
			string value = eValue.get_utf8().value.to_string();
			config[U(field.first)] = json::value::string(U(value));
		}
		arr[i] = config;
		i++;
	}
	return arr[0].at(U(key)).as_string();;
}

bool DBManager::isValidUser(string userId)
{
	cout << "DBManager::isValidUser" << endl;
	mongocxx::client client{mongocxx::uri{}};
	auto userCollection = client["library"]["users"];
	bsoncxx::stdx::optional<bsoncxx::document::value> result =
			userCollection.find_one(bsoncxx::builder::stream::document{} << userFields["userId"] << userId << finalize);
	if(!result) {
		cout << "Not a valid user for lend" << endl;
		return false;
	}
	return true;
}

bool DBManager::isValidBook(string bookCode)
{
	cout << "DBManager::isValidBook" << endl;
	mongocxx::client client{mongocxx::uri{}};
	auto bookCollection = client["library"]["books"];
	bsoncxx::stdx::optional<bsoncxx::document::value> result =
			bookCollection.find_one(bsoncxx::builder::stream::document{} << bookFields["code"] << bookCode << finalize);
	if(!result) {
		cout << "Not a valid book for lend" << endl;
		return false;
	}
	return true;
}

bool DBManager::isBookAvailable(string bookCode)
{
	cout << "DBManager::isBookAvailable" << endl;
	mongocxx::client client{mongocxx::uri{}};
	auto bookCollection = client["library"]["lends"];
	bsoncxx::stdx::optional<bsoncxx::document::value> result =
			bookCollection.find_one(bsoncxx::builder::stream::document{} << lendFields["bookCode"] << bookCode << lendFields["isReturned"] << "0" << finalize);
	if(result) {
		cout << "Book already lended" << endl;
		return false;
	}
	return true;
}

json::value DBManager::getUser(string userId)
{
	cout << "DBManager::getUser:" << userId << endl;
	mongocxx::client client{mongocxx::uri{}};
	auto collection = client["library"]["users"];
	mongocxx::cursor cursor = collection.find(document{} << userFields["userId"] << open_document
			<< "$eq" << userId
			<< close_document << finalize);

	json::value arr;
	int i = 0;
	for(auto v : cursor) {
		json::value user;
		for (std::pair<std::string, string> field : userFields)
		{
			bsoncxx::document::element eValue{v[field.first]};
			string value = eValue.get_utf8().value.to_string();
			user[U(field.first)] = json::value::string(U(value));
		}
		arr[i] = user;
		i++;
	}
	return arr[0];
}

int DBManager::getTotalLendCount()
{
	mongocxx::client client{mongocxx::uri{}};
	auto collection = client["library"]["lends"];
	bsoncxx::builder::stream::document document{};
	mongocxx::cursor cursor = collection.find({});

	int records = 0;
	for(auto v : cursor) {
		records++;
	}
	return records;
}

void DBManager::updateDelays(vector<pair<string, string>> kvPairs)
{
	cout << "DBManager::updateDelays" << endl;
	mongocxx::client client{mongocxx::uri{}};
	auto collection = client["library"]["lends"];

	if(getCurrentLends(kvPairs).size() > 0)
	{
		json::array lends = getCurrentLends(kvPairs).as_array();
		for(int i = 0; i < lends.size(); i++)
		{
			json::value lend = lends[i];
			string dueDate = lend.at(U(lendFields["dueDate"])).as_string();
			string lendId = lend.at(U(lendFields["lendId"])).as_string();
			string today = DateUtils::getCurrentDate();
			int delay = DateUtils::getDateDifference(dueDate, today);
			int finePerDay;
			int fine;
			try
			{
				finePerDay = stoi(getConfigValue("testlibrary", configFields["finePerDay"]));
				fine = delay * finePerDay;
			}
			catch(const std::invalid_argument& ia)
			{
				cout << "\nInvalid input for fine per day" << endl;
			}

			if(delay > 0)
			{
				collection.update_one(bsoncxx::builder::stream::document{}	<< lendFields["lendId"] << lendId << finalize,
						bsoncxx::builder::stream::document{} << "$set" << open_document
						<< lendFields["delay"] << to_string(delay)
						<< lendFields["fine"] << to_string(fine)
						<< close_document << finalize);

				notify(lend, delay);
			}
		}
	}
}

void DBManager::notify(json::value lend, int delay)
{
	SMSNotifier notifier;
	string code = lend.at(U(lendFields["bookCode"])).as_string();
	string userId = lend.at(U(lendFields["userId"])).as_string();
	json::value user = getUser(userId);
	string contact = user.at(U(userFields["contact"])).as_string();
	notifier.notify(contact, code, delay);
}
