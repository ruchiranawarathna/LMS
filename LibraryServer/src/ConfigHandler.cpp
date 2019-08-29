#include "ConfigHandler.h"

#include <iostream>
#include <fstream>

map<string, string> ConfigHandler::m_configMap;

bool ConfigHandler::init(string path)
{
	ifstream fileStream;
	fileStream.open(path.c_str());
	if (fileStream.fail()) {
		cerr << "Couldn't open config file [" + path + "] for reading" << endl;
		return false;
	}
	else
	{
		string line;
		while(getline(fileStream, line))
		{
			if(line[0] == '#' || line.empty())
				continue;
			auto delimiterPos = line.find("=");
			auto name = line.substr(0, delimiterPos);
			auto value = line.substr(delimiterPos + 1);
			m_configMap[name] = value;
		}
		fileStream.close();
	}
	return true;
}

string ConfigHandler::getConfigValue(string key)
{
	return m_configMap[key];
}
