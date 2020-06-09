#include <iostream>
#include <sstream>
#include <string>

#include <gtest/gtest.h>

#include <ltc/avector.hpp>

using namespace ltc;

class Test_avector : public ::testing::Test
{
protected:
    template<typename InputIter>
    std::string to_str(InputIter first, InputIter last)
    {
        std::stringstream ss;
        std::for_each(first, last, [&ss](const auto &i) { ss << i; });
        return ss.str();
    }
};

TEST_F(Test_avector, constructor)
{
    avector<int, 3> v;
    ASSERT_TRUE(v.empty());
    ASSERT_EQ(v.size(), 0);
    ASSERT_EQ(v.max_size(), 3);
    ASSERT_EQ(v.capacity(), 3);
    ASSERT_EQ(to_str(v.begin(), v.end()), "");
}

TEST_F(Test_avector, constructor_initializer_list)
{
    avector<int, 5> v = { 1, 2, 3 };
    ASSERT_FALSE(v.empty());
    ASSERT_EQ(v.size(), 3);
    ASSERT_EQ(v.max_size(), 5);
    ASSERT_EQ(v.capacity(), 5);
    ASSERT_EQ(to_str(v.begin(), v.end()), "123");
}

TEST_F(Test_avector, clear)
{
    avector<int, 5> v = { 1, 2, 3 };
    ASSERT_FALSE(v.empty());
    v.clear();
    ASSERT_TRUE(v.empty());
    ASSERT_EQ(v.size(), 0);
    ASSERT_EQ(v.capacity(), 5);
    ASSERT_EQ(to_str(v.begin(), v.end()), "");
}

TEST_F(Test_avector, insert_1)
{
    avector<int, 5> v = { 1, 2, 3 };
    ASSERT_EQ(v.size(), 3);
    ASSERT_EQ(v.at(0), 1);
    ASSERT_EQ(v.at(1), 2);
    ASSERT_EQ(v.at(2), 3);
    const int zero = 0;
    auto it = v.insert(v.begin() + 1, zero);
    ASSERT_EQ(*it, 0);
    ASSERT_EQ(v.size(), 4);
    ASSERT_EQ(v.at(0), 1);
    ASSERT_EQ(v.at(1), 0);
    ASSERT_EQ(v.at(2), 2);
    ASSERT_EQ(v.at(3), 3);
    ASSERT_EQ(to_str(v.begin(), v.end()), "1023");
}

TEST_F(Test_avector, insert_2)
{
    avector<int, 5> v = { 1, 2, 3 };
    int five = 5;
    auto it = v.insert(v.begin(), std::move(5));
    ASSERT_EQ(*it, 5);
    ASSERT_EQ(v.size(), 4);
    ASSERT_EQ(to_str(v.begin(), v.end()), "5123");
}

TEST_F(Test_avector, insert_3)
{
    avector<int, 5> v = { 1, 2, 3 };
    int five = 6;
    auto it = v.insert(v.end(), 2, 6);
    ASSERT_EQ(*it, 6);
    ASSERT_EQ(v.size(), 5);
    ASSERT_EQ(to_str(v.begin(), v.end()), "12366");
}
