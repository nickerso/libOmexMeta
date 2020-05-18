#include "gtest/gtest.h"
#include "semsim/RelandAPIWrappers/LibrdfWorld.h"

class LibrdfWorldTests : public ::testing::Test {

public:

    LibrdfWorldTests() = default;

};

TEST_F(LibrdfWorldTests, Test) {
    semsim::LibrdfWorld world;
    int actual = world.getRefCount();
    int expected = 1;
    ASSERT_EQ(actual, expected);
}

TEST_F(LibrdfWorldTests, TestCopyConstructor) {
    semsim::LibrdfWorld world1;
    semsim::LibrdfWorld world2 = world1;
    ASSERT_EQ(2, world1.getRefCount());
    ASSERT_EQ(2, world2.getRefCount());
    ASSERT_EQ(world1, world2);
}

TEST_F(LibrdfWorldTests, TestCopyAssignment) {
    semsim::LibrdfWorld world1;
    semsim::LibrdfWorld world2;
    world2 = world1;
    ASSERT_EQ(2, world1.getRefCount());
    ASSERT_EQ(2, world2.getRefCount());
    ASSERT_EQ(world1, world2);
}


TEST_F(LibrdfWorldTests, TestMoveConstructor) {
    semsim::LibrdfWorld world1;
    semsim::LibrdfWorld world2 = std::move(world1);
    ASSERT_EQ(1, world2.getRefCount());
    ASSERT_NE(world1, world2);
}

TEST_F(LibrdfWorldTests, TestMoveAssignment) {
    semsim::LibrdfWorld world1;
    semsim::LibrdfWorld world2;
    world2 = std::move(world1);
    ASSERT_EQ(1, world2.getRefCount());
    ASSERT_NE(world1, world2);
}







































