#include <algorithm>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>

#include <gtest/gtest.h>

#include <ltc/ltc.hpp>
#include <ltc/range.hpp>

using namespace ltc;

class Test_range : public ::testing::Test
{
};

TEST_F(Test_range, range)
{
    range<int> r(1, 10);
    std::stringstream ss;
    std::for_each(r.begin(), r.end(), [&ss](int i) { ss << i; });
    ASSERT_EQ(ss.str(), "123456789");
}

TEST_F(Test_range, reverse_range)
{
    reverse_range<int> r(0, 9);
    std::stringstream ss;
    std::for_each(r.begin(), r.end(), [&ss](int i) { ss << i; });
    ASSERT_EQ(ss.str(), "987654321");
}

TEST_F(Test_range, transform_range)
{
    range<int> int_range(1, 10);
    auto str_range = make_transform<std::string>(int_range.begin(), int_range.end(),
                                                 [](int i) { return std::to_string(i); });
    ASSERT_EQ(std::accumulate(str_range.begin(), str_range.end(), std::string()), "123456789");
}

TEST_F(Test_range, transform_range_reverse)
{
    reverse_range<int> int_range(0, 9);
    auto str_range = make_transform<std::string>(int_range.begin(), int_range.end(),
                                                 [](int i) { return std::to_string(i); });
    ASSERT_EQ(std::accumulate(str_range.begin(), str_range.end(), std::string()), "987654321");
}
