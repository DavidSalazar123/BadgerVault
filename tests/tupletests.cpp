#include "../Database/page.h"
#include <gtest/gtest.h>

TEST(TupleTest, GetWname)
{
    Tuple t;
    t.setWname("example.com");
    ASSERT_STREQ("example.com", t.getWname());
}

TEST(TupleTest, GetPassword)
{
    Tuple t;
    t.setPassword("password123");
    ASSERT_STREQ("password123", t.getPassword());
}

TEST(TupleTest, GetAndSetSize)
{
    Tuple t;
    t.setSize(10);
    ASSERT_EQ(10, t.getSize());
}

TEST(TupleTest, SetAndGetisEnd)
{
    Tuple t;
    t.setisEnd(10, 10);
    ASSERT_TRUE(t.getisEnd());
}
