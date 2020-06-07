#include <iostream>
#include <numeric>
#include <sstream>
#include <string>

#include <gtest/gtest.h>

#include <ltc/avector.hpp>

using namespace ltc;

class Test_avector : public ::testing::Test
{
protected:
    template <typename InputIter> std::string to_str(InputIter first, InputIter last)
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

TEST_F(Test_avector, iterators)
{
    avector<int, 5> v = { 1, 2, 3 };
    ASSERT_EQ(*v.begin(), 1);
    ASSERT_EQ(*v.rbegin(), 3);
    ASSERT_EQ(*(v.begin() + 1), 2);
    ASSERT_EQ(std::accumulate(v.begin(), v.end(), 0), 6);
    ASSERT_EQ(std::accumulate(v.rbegin(), v.rend(), 0), 6);
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
    avector<std::string, 5> v = { "1", "2", "3" };
    auto it = v.insert(v.end(), 2, "6");
    ASSERT_EQ(*it, "6");
    ASSERT_EQ(v.size(), 5);
    ASSERT_EQ(to_str(v.begin(), v.end()), "12366");
}

TEST_F(Test_avector, insert_4)
{
    avector<int, 6> v = { 1, 2, 3 };
    auto d = { 3, 2, 1 };
    v.insert(v.begin(), d.begin(), d.end());
    ASSERT_EQ(v.size(), 6);
    ASSERT_EQ(to_str(v.begin(), v.end()), "321123");
}

TEST_F(Test_avector, insert_5)
{
    avector<int, 6> v = { 1, 2, 3 };
    v.insert(v.begin(), { 3, 2, 1 });
    ASSERT_EQ(v.size(), 6);
    ASSERT_EQ(to_str(v.begin(), v.end()), "321123");
}

TEST_F(Test_avector, access)
{
    avector<int, 5> v = { 1, 2, 3 };
    ASSERT_EQ(v[0], 1);
    ASSERT_EQ(v[1], 2);
    ASSERT_EQ(v[2], 3);
    ASSERT_EQ(v.at(0), 1);
    ASSERT_EQ(v.at(1), 2);
    ASSERT_EQ(v.at(2), 3);
    ASSERT_EQ(v.front(), 1);
    ASSERT_EQ(v.back(), 3);
}

TEST_F(Test_avector, emplace)
{
    avector<std::string, 1> v = { "0" };
    const auto it = v.emplace(v.begin(), "1");
    ASSERT_EQ(*it, "1");
    ASSERT_EQ(v.size(), 1);
}

TEST_F(Test_avector, emplace_back)
{
    avector<std::string, 2> v = { "0" };
    const auto r = v.emplace_back("1");
    ASSERT_EQ(r, "1");
    ASSERT_EQ(v.size(), 2);
}

TEST_F(Test_avector, erase_1)
{
    avector<int, 5> v = { 1, 2, 3 };
    v.erase(v.begin()+1);
    ASSERT_EQ(v.size(), 2);
    ASSERT_EQ(to_str(v.begin(), v.end()), "13");
}

TEST_F(Test_avector, erase_2)
{
    avector<std::string, 5> v = { "1", "2", "3", "4", "5" };
    v.erase(v.begin(), v.end());
    ASSERT_EQ(to_str(v.begin(), v.end()), "");
}

TEST_F(Test_avector, erase_3)
{
    avector<std::string, 5> v = { "1", "2", "3", "4", "5" };
    v.erase(v.begin(), v.end() - 2);
    ASSERT_EQ(to_str(v.begin(), v.end()), "45");
}

TEST_F(Test_avector, erase_4)
{
    avector<std::string, 5> v = { "1", "2", "3", "4", "5" };
    v.erase(v.begin()+2, v.end());
    ASSERT_EQ(to_str(v.begin(), v.end()), "12");
}

TEST_F(Test_avector, erase_5)
{
    avector<std::string, 5> v = { "1", "2", "3", "4", "5" };
    v.erase(v.begin()+2, v.end() - 2);
    ASSERT_EQ(to_str(v.begin(), v.end()), "1245");
}

TEST_F(Test_avector, push_back_1)
{
    avector<std::string, 1> v;
    const std::string cval  = "1";
    v.push_back(cval);
    ASSERT_EQ(v.size(), 1);
    ASSERT_EQ(v.front(), cval);
}

TEST_F(Test_avector, push_back_2)
{
    avector<std::string, 1> v;
    std::string cval  = "1";
    v.push_back(std::move(cval));
    ASSERT_EQ(v.size(), 1);
    ASSERT_EQ(v.front(), "1");
    ASSERT_EQ(cval, "");
}

TEST_F(Test_avector, pop_back)
{
    avector<int, 5> v = { 1, 2, 3 };
    v.pop_back();
    ASSERT_EQ(to_str(v.begin(), v.end()), "12");
    v.pop_back(); 
    ASSERT_EQ(to_str(v.begin(), v.end()), "1");
    v.pop_back(); 
    ASSERT_TRUE(v.empty());
}

TEST_F(Test_avector, resize_1)
{
    avector<int, 5> v = { 1, 2, 3 };
    v.resize(5);
    ASSERT_EQ(v.size(), 5);
    ASSERT_EQ(to_str(v.begin(), v.end()), "12300");
}

TEST_F(Test_avector, resize_2)
{
    avector<int, 5> v = { 1, 2, 3, 4, 5 };
    v.resize(3);
    ASSERT_EQ(v.size(), 3);
    ASSERT_EQ(to_str(v.begin(), v.end()), "123");
}

TEST_F(Test_avector, resize_3)
{
    avector<int, 5> v = { 1, 2, 3 };
    v.resize(5, 9);
    ASSERT_EQ(v.size(), 5);
    ASSERT_EQ(to_str(v.begin(), v.end()), "12399");
}




