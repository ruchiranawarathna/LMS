#include <iostream>
#include <string>
#include <vector>

#include "HttpRequestParser.h"
#include "Server.h"
#include "utils/DateUtils.h"

using namespace std;

Server::Server(string bookUrl, string userUrl, string lendUrl, string configUrl) : m_bookServiceLilistener(bookUrl), m_userServiceLilistener(userUrl), m_lendServiceLilistener(lendUrl), m_configServiceLilistener(configUrl)
{
	m_bookServiceLilistener.support(methods::GET, std::bind(&Server::handle_get_books, this, std::placeholders::_1));
	m_bookServiceLilistener.support(methods::POST, std::bind(&Server::handle_post_book, this, std::placeholders::_1));
	m_bookServiceLilistener.support(methods::PUT, std::bind(&Server::handle_put_book, this, std::placeholders::_1));
	m_bookServiceLilistener.support(methods::DEL, std::bind(&Server::handle_del_book, this, std::placeholders::_1));

	m_userServiceLilistener.support(methods::GET, std::bind(&Server::handle_get_users, this, std::placeholders::_1));
	m_userServiceLilistener.support(methods::POST, std::bind(&Server::handle_post_user, this, std::placeholders::_1));
	m_userServiceLilistener.support(methods::PUT, std::bind(&Server::handle_put_user, this, std::placeholders::_1));
	m_userServiceLilistener.support(methods::DEL, std::bind(&Server::handle_del_user, this, std::placeholders::_1));

	m_lendServiceLilistener.support(methods::GET, std::bind(&Server::handle_get_lends, this, std::placeholders::_1));
	m_lendServiceLilistener.support(methods::POST, std::bind(&Server::handle_post_lend, this, std::placeholders::_1));
	m_lendServiceLilistener.support(methods::PUT, std::bind(&Server::handle_put_lend, this, std::placeholders::_1));

	m_configServiceLilistener.support(methods::PUT, std::bind(&Server::handle_put_configs, this, std::placeholders::_1));
}

void Server::init()
{
	DateUtils::init();
//	updateDelays();
}

void Server::handle_get_books(http_request request)
{
	cout << "handle get books" << endl;
	request.reply(status_codes::OK, getAllBooks(getKVPairs(request)));
}

void Server::handle_post_book(http_request request)
{
	cout << "handle post book" << endl;
	int result = m_dbManager.addBook(getKVPairs(request));
	if(result > 0)
	{
		request.reply(result, getAllBooks(getKVPairs(request)));
	}
	else
	{
		request.reply(status_codes::OK, getAllBooks(getKVPairs(request)));
	}
}

void Server::handle_post_lend(http_request request)
{
	cout << "handle_post_lend" << endl;
	
	int result = m_dbManager.addLend(getKVPairs(request));
	if(result > 0)
	{
		request.reply(result, getCurrentLends(getKVPairs(request)));
	}
	else
	{
		request.reply(status_codes::OK, getCurrentLends(getKVPairs(request)));
	}
}

void Server::handle_post_user(http_request request)
{
	cout << "handle_post_user" << endl;
	
	int result = m_dbManager.addUser(getKVPairs(request));
	if(result > 0)
	{
		request.reply(result, getAllUsers(getKVPairs(request)));
	}
	else
	{
		request.reply(status_codes::OK, getAllUsers(getKVPairs(request)));
	}
}

void Server::handle_del_book(http_request request)
{
	cout << "handle delete book" << endl;	
	m_dbManager.removeBook(getKVPairs(request));
	request.reply(status_codes::OK, getAllBooks(getKVPairs(request)));
}

void Server::handle_del_user(http_request request)
{
	cout << "handle_del_user" << endl;	
	m_dbManager.removeUser(getKVPairs(request));
	request.reply(status_codes::OK, getAllUsers(getKVPairs(request)));
}

void Server::handle_put_book(http_request request)
{
	cout << "handle put book" << endl;	
	m_dbManager.updateBook(getKVPairs(request));
	request.reply(status_codes::OK, getAllBooks(getKVPairs(request)));
}

void Server::handle_put_user(http_request request)
{
	cout << "handle_put_user" << endl;	
	m_dbManager.updateUser(getKVPairs(request));
	request.reply(status_codes::OK, getAllUsers(getKVPairs(request)));
}

void Server::handle_get_users(http_request request)
{
	cout << "handle_get_users" << endl;
	request.reply(status_codes::OK, getAllUsers(getKVPairs(request)));
}

void Server::handle_get_lends(http_request request)
{
	cout << "handle_get_lends" << endl;
	request.reply(status_codes::OK, getCurrentLends(getKVPairs(request)));
}

void Server::handle_put_lend(http_request request)
{
	cout << "handle_put_lend" << endl;	
	int result = m_dbManager.updateLend(getKVPairs(request));
	if(result > 0)
	{
		request.reply(result, getCurrentLends(getKVPairs(request)));
	}
	else
	{
		request.reply(status_codes::OK, getCurrentLends(getKVPairs(request)));
	}
}

void Server::handle_put_configs(http_request request)
{
	cout << "handle_put_configs" << endl;	
	int result = m_dbManager.updateConfigs(getKVPairs(request));
	m_dbManager.updateDelays(getKVPairs(request));
	request.reply(status_codes::OK, getCurrentLends(getKVPairs(request)));
}

json::value Server::getAllBooks(vector<pair<string, string>> kvPairs)
{
	return m_dbManager.getAllBooks(kvPairs);
}

json::value Server::getAllUsers(vector<pair<string, string>> kvPairs)
{
	return m_dbManager.getAllUsers(kvPairs);
}

json::value Server::getCurrentLends(vector<pair<string, string>> kvPairs)
{
	return m_dbManager.getCurrentLends(kvPairs);
}

void Server::updateDelays(vector<pair<string, string>> kvPairs)
{
	m_dbManager.updateDelays(kvPairs);
}

vector<pair<string, string>> Server::getKVPairs(http_request request)
{
	HttpRequestParser m_httpParser(request);
	return m_httpParser.getKeyValuePairs();
}
