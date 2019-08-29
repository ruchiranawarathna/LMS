#include <iostream>
#include <gtest/gtest.h>
#include "../src/db/DBManager.h"
#include "../src/Errors.h"
#include <cpprest/json.h>

class TestDBManager : public testing::Test {
public:
};

int getCount(json::value arr)
{
	int count = arr.as_array().size();
	return count;
}

TEST(TestDBManager, isValidBook)
{
	DBManager dbManager;
	bool result = dbManager.isValidBook("thisisnotavalidbook");
	EXPECT_EQ(false, result);
}

TEST(TestDBManager, isValidUser)
{
	DBManager dbManager;
	bool result = dbManager.isValidUser("thisisnotavaliduser");
	EXPECT_EQ(false, result);
}

/*
add a book: success
again add the book with same code: failure with code 801
remove the book: success
again add the book with same code: success
remove the book: success
*/
TEST(TestDBManager, addBook)
{
	DBManager dbManager;
	int recordCountBefore = getCount(dbManager.getAllBooks());
	cout << "Record Count Before:" << recordCountBefore << endl;
	vector<pair<string, string>> kvPairs;
	kvPairs.push_back(make_pair("code", "unit_test_book_code"));
	kvPairs.push_back(make_pair("title", "unit_test_book_title"));
	kvPairs.push_back(make_pair("author", "unit_test_book_author"));
	kvPairs.push_back(make_pair("category", "novel"));
	int result = dbManager.addBook(kvPairs);
	EXPECT_EQ(0, result);
	int recordCountAfter = getCount(dbManager.getAllBooks());
	EXPECT_EQ(recordCountBefore+1, recordCountAfter);
	//add duplicate book
	result = dbManager.addBook(kvPairs);
	EXPECT_EQ(801, result);
	//remove book
	result = dbManager.removeBook(kvPairs);
	EXPECT_EQ(0, result);
	//add duplicate book again
	result = dbManager.addBook(kvPairs);
	EXPECT_EQ(0, result);
	//remove book
	result = dbManager.removeBook(kvPairs);
	EXPECT_EQ(0, result);
}

/*
add a user: success
again add the user with same user ID: failure with code 800
remove the user: success
again add the user with same user ID: success
remove the user: success
*/
TEST(TestDBManager, addUser)
{
	DBManager dbManager;
	int recordCountBefore = getCount(dbManager.getAllUsers());
	vector<pair<string, string>> kvPairs;
	kvPairs.push_back(make_pair("userId", "test_user_id"));
	kvPairs.push_back(make_pair("name", "test_book_name"));
	kvPairs.push_back(make_pair("contact", "0771122333"));
	kvPairs.push_back(make_pair("address", "11,test_address"));
	int result = dbManager.addUser(kvPairs);
	EXPECT_EQ(0, result);
	int recordCountAfter = getCount(dbManager.getAllUsers());
	EXPECT_EQ(recordCountBefore+1, recordCountAfter);
	//add duplicate user
	result = dbManager.addUser(kvPairs);
	EXPECT_EQ(800, result);
	//remove user
	result = dbManager.removeUser(kvPairs);
	EXPECT_EQ(0, result);
	//add duplicate user again
	result = dbManager.addUser(kvPairs);
	EXPECT_EQ(0, result);
	//remove user
	result = dbManager.removeUser(kvPairs);
	EXPECT_EQ(0, result);
}

/*
add a user: success
update the user: success
remove the user: success
*/
TEST(TestDBManager, updateUser)
{
	DBManager dbManager;
	vector<pair<string, string>> kvPairs;
	kvPairs.push_back(make_pair("userId", "test_user_id"));
	kvPairs.push_back(make_pair("name", "test_book_name"));
	kvPairs.push_back(make_pair("contact", "0771122333"));
	kvPairs.push_back(make_pair("address", "11,test_address"));
	int result = dbManager.addUser(kvPairs);
	EXPECT_EQ(0, result);
	//update user
	kvPairs.clear();
	kvPairs.push_back(make_pair("userId", "test_user_id"));
	kvPairs.push_back(make_pair("name", "test_updated_book_name"));
	kvPairs.push_back(make_pair("contact", "0771122333"));
	kvPairs.push_back(make_pair("address", "11,test_address"));
	result = dbManager.updateUser(kvPairs);
	EXPECT_EQ(0, result);

	json::value user = dbManager.getUser("test_user_id");
	EXPECT_STREQ("test_updated_book_name", user.at(U("name")).as_string().c_str());

	//remove user
	result = dbManager.removeUser(kvPairs);
	EXPECT_EQ(0, result);
}
/*
add a user: success
get user by user ID: success
check the details are correct
remove the user: success
*/
TEST(TestDBManager, getUser)
{
	DBManager dbManager;
	vector<pair<string, string>> kvPairs;
	kvPairs.push_back(make_pair("userId", "test_user_id"));
	kvPairs.push_back(make_pair("name", "test_book_name"));
	kvPairs.push_back(make_pair("contact", "0771122333"));
	kvPairs.push_back(make_pair("address", "11,test_address"));
	int result = dbManager.addUser(kvPairs);
	EXPECT_EQ(0, result);

	json::value user = dbManager.getUser("test_user_id");
	EXPECT_STREQ("test_book_name", user.at(U("name")).as_string().c_str());
	EXPECT_STREQ("0771122333", user.at(U("contact")).as_string().c_str());
	EXPECT_STREQ("11,test_address", user.at(U("address")).as_string().c_str());
	//remove user
	result = dbManager.removeUser(kvPairs);
	EXPECT_EQ(0, result);
}

/*
add a book: success
add a user: success
add lend with invalid user: failure with code 802
add lend with invalid book: failure with code 803
add a valid lend: success
add lend with invalid already lended book: failure with code 804
return book: success
remove the user: success
remove the book: success
*/
TEST(TestDBManager, addLend)
{
	DBManager dbManager;

	cout << "===================== update config =========" << endl;
	vector<pair<string, string>> configKVPairs;
	configKVPairs.push_back(make_pair("libraryId", "testlibrary"));
	configKVPairs.push_back(make_pair("finePerDay", "10"));
	configKVPairs.push_back(make_pair("issuePeriod", "7"));
	int result = dbManager.updateConfigs(configKVPairs);
	EXPECT_EQ(0, result);

	cout << "===================== add book =========" << endl;
	vector<pair<string, string>> bookKVPairs;
	bookKVPairs.push_back(make_pair("code", "test_book_code"));
	bookKVPairs.push_back(make_pair("title", "test_book_title"));
	bookKVPairs.push_back(make_pair("author", "test_book_author"));
	bookKVPairs.push_back(make_pair("category", "novel"));
	result = dbManager.addBook(bookKVPairs);
	EXPECT_EQ(0, result);
	EXPECT_EQ(true, dbManager.isBookAvailable("test_book_code"));

	cout << "===================== add user =========" << endl;
	vector<pair<string, string>> userKVPairs;
	userKVPairs.push_back(make_pair("userId", "test_user_id"));
	userKVPairs.push_back(make_pair("name", "test_book_name"));
	userKVPairs.push_back(make_pair("contact", "0771122333"));
	userKVPairs.push_back(make_pair("address", "11,test_address"));
	result = dbManager.addUser(userKVPairs);
	EXPECT_EQ(0, result);

	vector<pair<string, string>> lendKVPairs;
	lendKVPairs.push_back(make_pair("lendId", "test_lend_id"));
	lendKVPairs.push_back(make_pair("userId", "invalid_test_user_id"));
	lendKVPairs.push_back(make_pair("bookCode", "test_book_name"));
	lendKVPairs.push_back(make_pair("isReturned", "0"));
	result = dbManager.addLend(lendKVPairs);
	EXPECT_EQ(802, result);

	lendKVPairs.clear();
	lendKVPairs.push_back(make_pair("lendId", "test_lend_id"));
	lendKVPairs.push_back(make_pair("userId", "test_user_id"));
	lendKVPairs.push_back(make_pair("bookCode", "invalid_test_book_name"));
	lendKVPairs.push_back(make_pair("isReturned", "0"));
	result = dbManager.addLend(lendKVPairs);
	EXPECT_EQ(803, result);

	int recordCountBefore = getCount(dbManager.getCurrentLends());
	string lendId = to_string(dbManager.getTotalLendCount() + 1);
	//valid lend
	cout << "===================== valid lend =========" << endl;
	lendKVPairs.clear();
	lendKVPairs.push_back(make_pair("lendId", lendId));
	lendKVPairs.push_back(make_pair("userId", "test_user_id"));
	lendKVPairs.push_back(make_pair("bookCode", "test_book_code"));
	lendKVPairs.push_back(make_pair("isReturned", "0"));
	result = dbManager.addLend(lendKVPairs);
	EXPECT_EQ(0, result);

	int recordCountAfter = getCount(dbManager.getCurrentLends());
	EXPECT_EQ(recordCountBefore+1, recordCountAfter);

	//lend already lended book
	cout << "=====================lend already lended book=========" << endl;
	result = dbManager.addLend(lendKVPairs);
	EXPECT_EQ(804, result);

	//return book
	cout << "=====================return book=========" << endl;
	lendKVPairs.clear();
	lendKVPairs.push_back(make_pair("lendId", lendId));
	lendKVPairs.push_back(make_pair("userId", "test_user_id"));
	lendKVPairs.push_back(make_pair("bookCode", "test_book_code"));
	lendKVPairs.push_back(make_pair("isReturned", "1"));
	result = dbManager.updateLend(lendKVPairs);
	EXPECT_EQ(0, result);

	recordCountAfter = getCount(dbManager.getCurrentLends());
	EXPECT_EQ(recordCountBefore, recordCountAfter);

	//remove book
	result = dbManager.removeBook(bookKVPairs);
	EXPECT_EQ(0, result);
	//remove user
	result = dbManager.removeUser(userKVPairs);
	EXPECT_EQ(0, result);
}

/*
add a config change: success
*/
TEST(TestDBManager, updateDelays)
{
	DBManager dbManager;

	vector<pair<string, string>> configKVPairs;
	configKVPairs.push_back(make_pair("libraryId", "testlibrary"));
	configKVPairs.push_back(make_pair("finePerDay", "19"));
	configKVPairs.push_back(make_pair("issuePeriod", "-1"));
	int result = dbManager.updateConfigs(configKVPairs);
	EXPECT_EQ(0, result);
	EXPECT_STREQ("19", dbManager.getConfigValue("testlibrary", "finePerDay").c_str());
	EXPECT_STREQ("-1", dbManager.getConfigValue("testlibrary", "issuePeriod").c_str());

	vector<pair<string, string>> bookKVPairs;
	bookKVPairs.push_back(make_pair("code", "test_book_code"));
	bookKVPairs.push_back(make_pair("title", "test_book_title"));
	bookKVPairs.push_back(make_pair("author", "test_book_author"));
	bookKVPairs.push_back(make_pair("category", "novel"));
	result = dbManager.addBook(bookKVPairs);
	EXPECT_EQ(0, result);
	EXPECT_EQ(true, dbManager.isBookAvailable("test_book_code"));

	vector<pair<string, string>> userKVPairs;
	userKVPairs.push_back(make_pair("userId", "test_user_id"));
	userKVPairs.push_back(make_pair("name", "test_book_name"));
	userKVPairs.push_back(make_pair("contact", "0771122333"));
	userKVPairs.push_back(make_pair("address", "11,test_address"));
	result = dbManager.addUser(userKVPairs);
	EXPECT_EQ(0, result);

	cout << "===================== add lend =========" << endl;
	string lendId = to_string(dbManager.getTotalLendCount() + 1);
	vector<pair<string, string>> lendKVPairs;
	lendKVPairs.push_back(make_pair("lendId", lendId));
	lendKVPairs.push_back(make_pair("userId", "test_user_id"));
	lendKVPairs.push_back(make_pair("bookCode", "test_book_code"));
	lendKVPairs.push_back(make_pair("isReturned", "0"));
	result = dbManager.addLend(lendKVPairs);
	EXPECT_EQ(0, result);

	dbManager.updateDelays();
	json::array lends = dbManager.getCurrentLends().as_array();
	cout << "===================== iterate =========:" + lends.size() << endl;
	string fine;
	for(int i = 0; i < lends.size(); i++)
	{
		json::value lend = lends[i];
		string id = lend.at(U("lendId")).as_string();

		if(id == lendId)
		{
			cout << "===================== lend found =========" << endl;
			fine = lend.at(U("fine")).as_string();
		}
	}
	EXPECT_STREQ("19", fine.c_str());

	//return book
	cout << "=====================return book=========" << endl;
	lendKVPairs.clear();
	lendKVPairs.push_back(make_pair("lendId", lendId));
	lendKVPairs.push_back(make_pair("userId", "test_user_id"));
	lendKVPairs.push_back(make_pair("bookCode", "test_book_code"));
	lendKVPairs.push_back(make_pair("isReturned", "1"));
	result = dbManager.updateLend(lendKVPairs);
	EXPECT_EQ(0, result);

	//remove book
	result = dbManager.removeBook(bookKVPairs);
	EXPECT_EQ(0, result);
	//remove user
	result = dbManager.removeUser(userKVPairs);
	EXPECT_EQ(0, result);
}

/*
add a config change: success
*/
TEST(TestDBManager, updateConfig)
{
	DBManager dbManager;

	vector<pair<string, string>> configKVPairs;
	configKVPairs.push_back(make_pair("libraryId", "testlibrary"));
	configKVPairs.push_back(make_pair("finePerDay", "7"));
	configKVPairs.push_back(make_pair("issuePeriod", "5"));
	int result = dbManager.updateConfigs(configKVPairs);
	EXPECT_EQ(0, result);
	EXPECT_STREQ("7", dbManager.getConfigValue("testlibrary", "finePerDay").c_str());
	EXPECT_STREQ("5", dbManager.getConfigValue("testlibrary", "issuePeriod").c_str());
}
