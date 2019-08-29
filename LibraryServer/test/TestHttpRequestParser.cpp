#include <iostream>
#include <gtest/gtest.h>
#include "../src/HttpRequestParser.h"

class TestHttpRequestParser : public testing::Test {
public:
};

TEST(TestHttpRequestParser, splitString)
{
	HttpRequestParser parser;
	std::vector<string> segments;
	string str = "code=111&title=rrrrrrr&author=d&category=sf";
	parser.splitString(str, segments, '&');
	EXPECT_EQ(4, segments.size());

}
