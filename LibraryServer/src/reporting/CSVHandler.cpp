#include "CSVHandler.h"

#include <iostream>
#include <fstream>

void CSVHandler::writeToCSV(vector<string> rows, string header, string path)
{
	ofstream writer;
	writer.open(path);
	writer << header << endl;
	for(string line : rows)
	{
		writer << line << endl;
	}
    writer.close();
}


