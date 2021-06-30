/** A unit-test for obj_pool implementation.
 *
 * @author Vitalii Chernookyi
 */
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstddef>
#include <cstdarg>
#include <cassert>

#include <iostream>

#include <gtest/gtest.h>

#include "object-pool.hpp"

// Example of an poolable object
class UsedObject : public PoolableObject {
public:
    UsedObject(int init_num = 0) : _num{init_num}, _init_num{init_num} {};
    void increment() { ++_num; }

    void print() { std::cout << "My number is: " << _num << std::endl; }
    void reset() override { _num = _init_num; }

private:
    int _num;
    int _init_num;
};

class ObjectPoolFixture : public ::testing::Test {
 protected:
  // Per-test-suite set-up.
  // Called before the first test in this test suite.
  // Can be omitted if not needed.
  static void SetUpTestSuite() {
	  obj_pool = new ObjectPool<UsedObject, int> (0, 2, 5);
  }

  // Per-test-suite tear-down.
  // Called after the last test in this test suite.
  // Can be omitted if not needed.
  static void TearDownTestSuite() {
	  delete obj_pool;
  }

  // You can define per-test set-up logic as usual.
  //virtual void SetUp() { ... }

  // You can define per-test tear-down logic as usual.
  //virtual void TearDown() { ... }

  // Some expensive resource shared by all tests.
  static ObjectPool<UsedObject, int> *obj_pool;
};

ObjectPool<UsedObject, int> *ObjectPoolFixture::obj_pool = NULL;

TEST_F(ObjectPoolFixture, Smoke) {
	ASSERT_NE(nullptr, obj_pool);
	// "Initialized object pool with max size of "
	ASSERT_EQ(2U, obj_pool->max_size());
	// "Size of the pool: "
	ASSERT_EQ(0U, obj_pool->size());
	// "Available objects: "
	ASSERT_EQ(2U, obj_pool->free());

    auto inst = obj_pool->get();
    inst->increment();
    inst->print();

	// "Size of the pool: "
	ASSERT_EQ(1U, obj_pool->size());
	// "Available objects: "
	ASSERT_EQ(1U, obj_pool->free());

    {
        auto inst2 = obj_pool->get();
        inst->increment();
        inst->print();
		std::cout << "Instance 2 pointer is " << inst2.get() << std::endl;

		// "Size of the pool: "
		ASSERT_EQ(2U, obj_pool->size());
		// "Available objects: "
		ASSERT_EQ(0U, obj_pool->free());
	}

	// "Size of the pool: "
	ASSERT_EQ(2U, obj_pool->size());
	// "Available objects: "
	ASSERT_EQ(1U, obj_pool->free());

    auto inst2 = obj_pool->get();
    std::cout << "Instance 2 pointer is " << inst2.get() << std::endl;

    auto inst3 = obj_pool->get();
    ASSERT_TRUE(inst3 == nullptr);
	// "Pool is empty, all objects are in use"
    ASSERT_TRUE(obj_pool->empty());
}
