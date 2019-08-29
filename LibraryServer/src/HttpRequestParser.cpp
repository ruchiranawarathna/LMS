#include <iostream>
#include <utility>
#include "HttpRequestParser.h"

using namespace std;

HttpRequestParser::HttpRequestParser(http_request request): m_request(request){}

void HttpRequestParser::splitString(string str, vector<string>& values, char delimiter)
{
	istringstream  stream(str);
	string segment;

	while(getline(stream, segment, delimiter))
	{
		values.push_back(segment);
	}
}

void HttpRequestParser::findAndReplace(string& original, string out, string in)
{
    string::size_type pos = 0u;
    while ((pos = original.find(out, pos)) != string::npos)
    {	original.replace(pos, out.length(), in);
	    pos += in.length();
    }
}

string HttpRequestParser::getResponseParameters(string requestStr)
{
	stringstream tokenStream(requestStr);
	string paramStr;
	getline(tokenStream, paramStr, '\n');
	paramStr = paramStr.substr(paramStr.find("?") + 1);
	stringstream paramStream(paramStr);
	getline(paramStream, paramStr, ' ');
	findAndReplace(paramStr, "%20", " ");
	return paramStr;
}

vector<pair<string, string>> HttpRequestParser::getKeyValuePairs()
{
	string paramStr = getResponseParameters(m_request.to_string());
	vector<pair<string, string>> kvPairs;
	vector<string> parameters;
	splitString(paramStr, parameters, '&');
	for(string kv : parameters)
	{
		vector<string> kvPair;
		splitString(kv, kvPair, '=');
		if(kvPair.size() == 2)
		{
			kvPairs.push_back(make_pair(kvPair[0], kvPair[1]));
		}
	}
	cout << "HttpRequestParser::getKeyValuePairs:" << kvPairs.size() << endl;
	return kvPairs;
}
