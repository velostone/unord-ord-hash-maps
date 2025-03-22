#include "gtest.h"
#include "maps.h"

using namespace maps;

TEST(UnorderedMapTest, can_create_unordered_map)
{
	ASSERT_NO_THROW(UnorderedTable<int> M);
}