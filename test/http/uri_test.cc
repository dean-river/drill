#include "gtest/gtest.h"
#include <drill/http/uri.h>
#include <string>

TEST(uri, Simple_1) {
    drill::http::Uri s("http://192.168.145.223:80/");
	std::string t = s.toString();
    ASSERT_EQ(t, "http://192.168.145.223/");
}

TEST(uri, Simple_2) {
    drill::http::Uri s("https://192.168.145.223/");
	std::string t = s.toString();
    ASSERT_EQ(t, "https://192.168.145.223/");
}

TEST(uri, Simple_3) {
    drill::http::Uri s("https://192.168.145.223/htm");
	std::string t = s.toString();
    ASSERT_EQ(t, "https://192.168.145.223/htm");
}

TEST(uri, Simple_4) {
    drill::http::Uri s("/hello");
	std::string t = s.toString();
    ASSERT_EQ(t, "/hello");
}