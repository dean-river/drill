#include "gtest/gtest.h"
#include <drill/common/lru_cache.h>
#include <string>

TEST(Lru_cache, Simple) {
    drill::common::LruCache<std::string, std::string> cache;

	cache.put("dog", "da gou");
	cache.put("cat", "da mao");

	size_t s = 2;
    ASSERT_EQ(cache.size(), s);
}

