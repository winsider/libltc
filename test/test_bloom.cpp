#include <gtest/gtest.h>

#include <ltc/bloom.hpp>

using namespace ltc;

class Test_bloom : public ::testing::Test
{
protected:
};

TEST_F(Test_bloom, constructor)
{
    bloom_filter<int> filter(1000, 2);
    ASSERT_EQ(filter.size(), 1000);
}

TEST_F(Test_bloom, add)
{
    bloom_filter<int> filter(1000, 6);
    for (auto i = 0; i < 1000000; ++i)
    {
        if (!(i % 7))
        {
            filter.add(i);
        }
    }
    ASSERT_EQ(filter.count(), 142858);
}

TEST_F(Test_bloom, try_add)
{
    bloom_filter<int> filter(10000000, 6);
    auto not_in_filter = 0;
    auto in_filter = 0;
    for (auto i = 0; i < 1000000; ++i)
    {
        if (!(i % 7))
        {
            if (filter.try_add(i))
            {
                ++not_in_filter;
            }
            else
            {
                ++in_filter;
            }            
        }
    }
    ASSERT_LE(filter.count(), 142858);
}

TEST_F(Test_bloom, possibly_contains)
{
    bloom_filter<int> filter(10000000, 6);
    for (auto i = 0; i < 1000000; ++i)
    {
        if (!(i % 7))
        {
            filter.add(i);
        }
    }

    auto not_in_filter = 0;
    auto in_filter = 0;

    for (auto i = 0; i < 1000000; ++i)
    {
        if (!(i % 7))
        {
            ASSERT_TRUE(filter.possibly_contains(i));
            ++in_filter;
        }
        else if (!filter.possibly_contains(i))
        {
            ++not_in_filter;
        }
    }
    ASSERT_EQ(filter.count(), in_filter);
    ASSERT_GT(not_in_filter, in_filter);
}

TEST_F(Test_bloom, calculator)
{
    bloom_calculator c;
    ASSERT_EQ(c.items(), 4000);
}