#include <iostream>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#pragma comment(lib, "cpprest_2_10")

#include "Server.h"
#include <mongocxx/instance.hpp>

#include "HttpRequestParser.h"
#include "Server.h"
#include "ConfigHandler.h"

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

int main()
{
	if(ConfigHandler::init("/home/hsenid/training/angularProj/library-management-system-backend/LibraryServer/configs.config") == false)
	{
		cout << "Configuration Error" << endl;
		exit(0);
	}

	mongocxx::instance inst{};
	string bookUrl = ConfigHandler::getConfigValue("BOOK_SERVICE_URL");
	string userUrl = ConfigHandler::getConfigValue("USER_SERVICE_URL");
	string lendUrl = ConfigHandler::getConfigValue("LEND_SERVICE_URL");
	string configUrl = ConfigHandler::getConfigValue("CONFIG_SERVICE_URL");
	std::unique_ptr<Server> server = std::unique_ptr<Server>(new Server(bookUrl, userUrl, lendUrl, configUrl));
	server->init();

	try
	{
		server->openBookService().wait();
		cout << "Listening to book requests..." << endl;

		server->openUserService().wait();
		cout << "Listening to user requests..." << endl;

		server->openLendService().wait();
		cout << "Listening to lend requests..." << endl;

		server->openConfigService().wait();
		cout << "Listening to config requests..." << endl;

		while (true);
	}
	catch (exception const & e)
	{
		wcout << e.what() << endl;
	}

   return 0;
}


