#include <gtest/gtest.h>

#include <ltc/btree.hpp>

using namespace ltc;

class Test_btree : public ::testing::Test
{
protected:
};

TEST_F(Test_btree, at)
{
    btree t{20};
    EXPECT_THROW(t.at("test"), std::out_of_range);
}

TEST_F(Test_btree, insert)
{
    btree t{20};
    for (int i=0; i<20; ++i)
    {
        t.insert(std::make_pair(std::to_string(i),"v"));
    }
    EXPECT_THROW(t.insert(std::make_pair("21","v")), std::runtime_error);
}
