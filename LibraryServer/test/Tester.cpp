#include <iostream>
#pragma comment(lib, "cpprest_2_10")

#include <mongocxx/instance.hpp>
#include <gtest/gtest.h>
#include "../src/utils/DateUtils.h"
#include "../src/ConfigHandler.h"

int main(int argc, char **argv) {
	if(ConfigHandler::init("/home/hsenid/training/angularProj/library-management-system-backend/LibraryServer/configs.config") == false)
	{
		cout << "Configuration Error" << endl;
		exit(0);
	}

	DateUtils::init();
	mongocxx::instance inst{};
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
