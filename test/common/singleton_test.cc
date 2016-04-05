#include <drill/common/singleton.h>
#include "gtest/gtest.h"

using namespace drill::common;

class stest:public Singleton<stest> {
public:
	stest():i(0) {}
	int i;
	
};

TEST(singleton1, 1) {

	stest *instance1 = stest::GetInstance();
	stest *instance2 = stest::GetInstance();
	EXPECT_EQ(instance1,instance2);

}

