#include <drill/common/math.h>
#include "gtest/gtest.h"

using namespace drill::common;

TEST(integer_log2_floor, 1) {
	int ret = integer_log2_floor(9);
	EXPECT_EQ(3, ret);
}

TEST(integer_log2_ceil, 1) {
	int ret = integer_log2_ceil(9);
	EXPECT_EQ(4, ret);
}

