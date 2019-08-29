#ifndef HTTPREQUESTPARSER_H
#define HTTPREQUESTPARSER_H

#include <iostream>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#pragma comment(lib, "cpprest_2_10")

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

using namespace std;

class HttpRequestParser
{
public:
	HttpRequestParser(){};
	HttpRequestParser(http_request request);
	~HttpRequestParser(){}
	vector<pair<string, string>> getKeyValuePairs();
	void splitString(string str, vector<string>& values, char delimiter);

private:
	http_request m_request;
	string getResponseParameters(string requestStr);
	void findAndReplace(string& original, string out, string in);
};


#endif
