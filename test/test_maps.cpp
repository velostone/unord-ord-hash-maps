#include "gtest.h"
#include "maps.h"

using namespace maps;

// general tests for UndorderedTable
TEST(UnorderedMapTest, can_create_unordered_map)
{
	ASSERT_NO_THROW(UnorderedTable<int> M);
}
TEST(UnorderedMapTest, can_insert_elem_with_uniq_key)
{
	UnorderedTable<int> M;
	M.insert(4, 7);
	M.insert(7, 12);
	M.insert(9, 1);
	std::pair<size_t, int> row_ = std::make_pair(4, 7);
	EXPECT_EQ(M[0], row_);
	row_ = std::make_pair(7, 12);
	EXPECT_EQ(M[1], row_);
	row_ = std::make_pair(9, 1);
	EXPECT_EQ(M[2], row_);
}
TEST(UnorderedMapTest, insertion_change_size_correctly)
{
	UnorderedTable<int> M;
	M.insert(4, 7);
	M.insert(7, 12);
	M.insert(9, 1);
	EXPECT_EQ(M.getSize(), 3);
}

// typed fixture for UndorderedTable
template<class T>
class TUnorderedMapTest : public ::testing::Test
{
protected:
	UnorderedTable<T>* p;
	std::pair<size_t, T>* row;
public:
	void SetUp() override
	{
		p = new UnorderedTable<T>;
		p->insert(4, 7);
		p->insert(7, 12);
		p->insert(9, 1);
		row = new std::pair<size_t, T>;
		*row = std::make_pair(4, 7);
	}
	void TearDown() override
	{
		delete p;
		p = nullptr;
		delete row;
		row = nullptr;
	}
	void SetRow(std::pair<size_t, T> new_row)
	{
		*row = new_row;
	}
};

typedef ::testing::Types<int, float, double> types;
TYPED_TEST_CASE(TUnorderedMapTest, types);

// typed tests for UndorderedTable
TYPED_TEST(TUnorderedMapTest, cant_insert_elem_with_non_uniq_key)
{
	EXPECT_FALSE(this->p->insert(4, 12));
}
TYPED_TEST(TUnorderedMapTest, can_find_existing_elem)
{
	this->SetRow(std::make_pair(7, 12));
	EXPECT_EQ((this->p->find(7)).value(), *(this->row));
}
TYPED_TEST(TUnorderedMapTest, cant_find_non_existing_elem)
{
	EXPECT_FALSE((this->p->find(2)).has_value());
}
TYPED_TEST(TUnorderedMapTest, can_find_idx_of_existing_elem)
{
	EXPECT_EQ((this->p->find_idx(7)).value(), 1);
}
TYPED_TEST(TUnorderedMapTest, cant_find_idx_of_non_existing_elem)
{
	EXPECT_FALSE((this->p->find(2)).has_value());
}
TYPED_TEST(TUnorderedMapTest, can_erase_existing_elem)
{
	this->p->erase(7);
	EXPECT_FALSE(this->p->find(7).has_value());
}
TYPED_TEST(TUnorderedMapTest, cant_erase_non_existing_elem)
{
	EXPECT_FALSE(this->p->erase(10));
}
TYPED_TEST(TUnorderedMapTest, erasing_change_size_correctly)
{
	this->p->erase(4);
	this->p->erase(7);
	EXPECT_EQ(this->p->getSize(), 1);
}

// general tests for OrderedTable
TEST(OrderedMapTest, can_create_ordered_map)
{
	ASSERT_NO_THROW(OrderedTable<int> M);
}
TEST(OrderedMapTest, can_insert_elem_with_uniq_key)
{
	OrderedTable<int> M;
	M.insert(4, 7);
	M.insert(7, 12);
	M.insert(9, 1);
	std::pair<size_t, int> row_ = std::make_pair(4, 7);
	EXPECT_EQ(M[0], row_);
	row_ = std::make_pair(7, 12);
	EXPECT_EQ(M[1], row_);
	row_ = std::make_pair(9, 1);
	EXPECT_EQ(M[2], row_);
}
TEST(OrderedMapTest, insertion_change_size_correctly)
{
	OrderedTable<int> M;
	M.insert(4, 7);
	M.insert(7, 12);
	M.insert(9, 1);
	EXPECT_EQ(M.getSize(), 3);
}
TEST(OrderedMapTest, insertion_save_correct_order)
{
	OrderedTable<int> M;
	M.insert(9, 1);
	M.insert(4, 7);
	M.insert(7, 12);
	std::pair<size_t, int> row_ = std::make_pair(4, 7);
	EXPECT_EQ(M[0], row_);
	row_ = std::make_pair(7, 12);
	EXPECT_EQ(M[1], row_);
	row_ = std::make_pair(9, 1);
	EXPECT_EQ(M[2], row_);
}

// typed fixture for OrderedTable
template<class T>
class TOrderedMapTest : public ::testing::Test
{
protected:
	OrderedTable<T>* p;
	std::pair<size_t, T>* row;
public:
	void SetUp() override
	{
		p = new OrderedTable<T>;
		p->insert(4, 7);
		p->insert(7, 12);
		p->insert(9, 1);
		row = new std::pair<size_t, T>;
		*row = std::make_pair(4, 7);
	}
	void TearDown() override
	{
		delete p;
		p = nullptr;
		delete row;
		row = nullptr;
	}
	void SetRow(std::pair<size_t, T> new_row)
	{
		*row = new_row;
	}
};

typedef ::testing::Types<int, float, double> types;
TYPED_TEST_CASE(TOrderedMapTest, types);

// typed tests for OrderedTable
TYPED_TEST(TOrderedMapTest, cant_insert_elem_with_non_uniq_key)
{
	EXPECT_FALSE(this->p->insert(4, 12));
}
TYPED_TEST(TOrderedMapTest, can_find_existing_elem)
{
	this->SetRow(std::make_pair(7, 12));
	EXPECT_EQ((this->p->find(7)).value(), *(this->row));
}
TYPED_TEST(TOrderedMapTest, cant_find_non_existing_elem)
{
	EXPECT_FALSE((this->p->find(2)).has_value());
}
TYPED_TEST(TOrderedMapTest, can_erase_existing_elem)
{
	this->p->erase(7);
	EXPECT_FALSE(this->p->find(7).has_value());
}
TYPED_TEST(TOrderedMapTest, cant_erase_non_existing_elem)
{
	EXPECT_FALSE(this->p->erase(10));
}
TYPED_TEST(TOrderedMapTest, erasing_change_size_correctly)
{
	this->p->erase(4);
	this->p->erase(7);
	EXPECT_EQ(this->p->getSize(), 1);
}
TYPED_TEST(TOrderedMapTest, erasing_save_correct_order)
{
	this->p->erase(7);
	EXPECT_EQ((*(this->p))[0], *(this->row));
	this->SetRow(std::make_pair(9, 1));
	EXPECT_EQ((*(this->p))[1], *(this->row));
}

// general tests for HashTable
TEST(HashMapTest, can_create_hash_map)
{
	ASSERT_NO_THROW(HashTable<int> M);
}
TEST(HashMapTest, can_create_specific_size_hash_map)
{
	ASSERT_NO_THROW(HashTable<int> M(150));
}
TEST(HashMapTest, can_insert_elem_with_uniq_key)
{
	HashTable<int> M;
	M.insert(4, 7);
	M[4];
	M.insert(7, 12);
	M.insert(9, 1);
	std::pair<size_t, int> row_ = std::make_pair(4, 7);
	EXPECT_EQ(M[4], row_);
	row_ = std::make_pair(7, 12);
	EXPECT_EQ(M[7], row_);
	row_ = std::make_pair(9, 1);
	EXPECT_EQ(M[9], row_);
}
TEST(HashMapTest, insertion_change_size_correctly)
{
	HashTable<int> M;
	M.insert(4, 7);
	M.insert(7, 12);
	M.insert(9, 1);
	EXPECT_EQ(M.getSize(), 3);
}

// typed fixture for HashTable
template<class T>
class THashMapTest : public ::testing::Test
{
protected:
	HashTable<T>* p;
	std::pair<size_t, T>* row;
public:
	void SetUp() override
	{
		p = new HashTable<T>;
		p->insert(4, 7);
		p->insert(7, 12);
		p->insert(9, 1);
		row = new std::pair<size_t, T>;
		*row = std::make_pair(4, 7);
	}
	void TearDown() override
	{
		delete p;
		p = nullptr;
		delete row;
		row = nullptr;
	}
	void SetRow(std::pair<size_t, T> new_row)
	{
		*row = new_row;
	}
	void MakeCollision()
	{
		// h(4) == h(297) == h(769) == 89
		p->insert(297, 15);
		p->insert(769, 3);
	}
};

typedef ::testing::Types<int, float, double> types;
TYPED_TEST_CASE(THashMapTest, types);

// typed tests for HashTable
TYPED_TEST(THashMapTest, cant_insert_elem_with_non_uniq_key)
{
	EXPECT_FALSE(this->p->insert(4, 12));
}
TYPED_TEST(THashMapTest, can_find_existing_elem)
{
	this->SetRow(std::make_pair(7, 12));
	EXPECT_EQ((this->p->find(7)).value(), *(this->row));
}
TYPED_TEST(THashMapTest, cant_find_non_existing_elem)
{
	EXPECT_FALSE((this->p->find(2)).has_value());
}
TYPED_TEST(THashMapTest, can_erase_existing_elem)
{
	this->p->erase(7);
	EXPECT_FALSE(this->p->find(7).has_value());
}
TYPED_TEST(THashMapTest, cant_erase_non_existing_elem)
{
	EXPECT_FALSE(this->p->erase(10));
}
TYPED_TEST(THashMapTest, erasing_change_size_correctly)
{
	this->p->erase(4);
	this->p->erase(7);
	EXPECT_EQ(this->p->getSize(), 1);
}
TYPED_TEST(THashMapTest, can_create_hash_of_elem)
{
	ASSERT_NO_THROW(this->p->h(15));
}
TYPED_TEST(THashMapTest, can_insert_elem_with_collision)
{
	this->MakeCollision();
	EXPECT_EQ((*(this->p))[4], *(this->row));
	this->SetRow(std::make_pair(297, 15));
	EXPECT_EQ((*(this->p))[297], *(this->row));
	this->SetRow(std::make_pair(769, 3));
	EXPECT_EQ((*(this->p))[769], *(this->row));
};
TYPED_TEST(THashMapTest, can_find_elem_with_collision)
{
	this->MakeCollision();
	this->SetRow(std::make_pair(297, 15));
	EXPECT_EQ((this->p->find(297)).value(), *(this->row));
};
TYPED_TEST(THashMapTest, can_erase_elem_with_collision)
{
	this->MakeCollision();
	this->p->erase(769);
	EXPECT_FALSE((this->p->find(769)).has_value());
};

