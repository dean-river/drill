#include <iostream>
#include "gtest/gtest.h"
#include <drill/common/string.h>

using namespace drill::common;

TEST(toupper, to_upper_1) {
	std::string a("abc");
	toupper(a);
	EXPECT_EQ(a,"ABC");
}

TEST(toupper, to_upper_2) {
	std::string a("aBc");
	toupper(a);
	EXPECT_EQ(a,"ABC");
}

TEST(toupper, to_upper_3) {
	std::string a("aBc");
	toupper(a);
	EXPECT_EQ(a,"ABC");
}

TEST(tolower, to_lower_1) {
	std::string a("aBc");
	tolower(a);
	EXPECT_EQ(a,"abc");
}

TEST(start_with, 1) {
	std::string str("match me hello");
	std::string m("match ");
	bool ret = start_with(str, m);
	EXPECT_TRUE(ret);
}

TEST(start_with, 2) {
	std::string str("match me hello");
	std::string m("matchm");
	bool ret = start_with(str, m);
	EXPECT_FALSE(ret);
}


TEST(end_with, 1) {
	std::string str("match me hello");
	std::string m("hello");
	bool ret = end_with(str, m);
	EXPECT_TRUE(ret);
}

TEST(end_with, 2) {
	std::string str("match me hello");
	std::string m("hello ");
	bool ret = end_with(str, m);
	EXPECT_FALSE(ret);
}

TEST(parse_size, 1) {
	uint64_t ret;
	std::string n("10 KB");
	parse_size(n.data(), ret);
	EXPECT_EQ(ret, (uint64_t)10000);
}

TEST(parse_size, 3) {
	uint64_t ret;
	std::string n("10KB");
	parse_size(n.data(), ret);
	EXPECT_EQ(ret, (uint64_t)10000);
}
TEST(parse_size, 2) {
	uint64_t ret;
	std::string n("10KIB");
	parse_size(n.data(), ret);
	EXPECT_EQ(ret, (uint64_t)10240);
}

TEST(parse_size, 4) {
	uint64_t ret;
	std::string n("10 KIB");
	parse_size(n.data(), ret);
	EXPECT_EQ(ret, (uint64_t)10240);
}

TEST(hex_encode_decode, 1) {
	std::string buff="hello world";
	std::string x = hex_encode(buff.data(), buff.size());
	std::string xx = hex_decode(x.data(), x.size());
	EXPECT_EQ(buff, xx);
}

TEST(hex_encode_decode, 2) {
	uint64_t buff=10245;
	std::string x = hex_encode(&buff);
	std::string xx = hex_decode(x.data(), x.size());
	std::string y = hex_encode(xx);

	EXPECT_EQ(y, x);
}

TEST(trim, 1) {
	std::string buff = " a b b d 	\n";
	std::string x = trim(buff);
	EXPECT_EQ("a b b d", x);
}



