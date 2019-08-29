#ifndef LIBRARY_MANAGEMENT_SYSTEM_BACKEND_LIBRARYSERVER_SRC_SERVER_H_
#define LIBRARY_MANAGEMENT_SYSTEM_BACKEND_LIBRARYSERVER_SRC_SERVER_H_

#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include "HttpRequestParser.h"
#include "db/DBManager.h"
#pragma comment(lib, "cpprest_2_10")

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

using namespace std;

class Server
{
public:
	Server(string bookUrl, string userUrl, string lendUrl, string configUrl);
	~Server(){}
	void init();
	pplx::task<void> openBookService() {return m_bookServiceLilistener.open();}
	pplx::task<void> openUserService() {return m_userServiceLilistener.open();}
	pplx::task<void> openLendService() {return m_lendServiceLilistener.open();}
	pplx::task<void> openConfigService() {return m_configServiceLilistener.open();}
	pplx::task<void> closeBookService() {return m_bookServiceLilistener.close();}
	pplx::task<void> closeUserService() {return m_userServiceLilistener.close();}
	pplx::task<void> closeLendService() {return m_lendServiceLilistener.close();}
	pplx::task<void> closeConfigService() {return m_configServiceLilistener.close();}

private:
	void handle_get_books(http_request request);
	void handle_post_book(http_request request);
	void handle_put_book(http_request request);
	void handle_del_book(http_request request);

	void handle_get_users(http_request request);
	void handle_post_user(http_request request);
	void handle_put_user(http_request request);
	void handle_del_user(http_request request);

	void handle_get_lends(http_request request);
	void handle_post_lend(http_request request);
	void handle_put_lend(http_request request);

	void handle_put_configs(http_request request);

	json::value getAllBooks(vector<pair<string, string>> kvPairs);
	json::value getAllUsers(vector<pair<string, string>> kvPairs);
	json::value getCurrentLends(vector<pair<string, string>> kvPairs);

	void updateDelays(vector<pair<string, string>> kvPairs);
	vector<pair<string, string>> getKVPairs(http_request request);

	http_listener m_bookServiceLilistener;
	http_listener m_userServiceLilistener;
	http_listener m_lendServiceLilistener;
	http_listener m_configServiceLilistener;

	DBManager m_dbManager;
};

#endif /* LIBRARY_MANAGEMENT_SYSTEM_BACKEND_LIBRARYSERVER_SRC_SERVER_H_ */
