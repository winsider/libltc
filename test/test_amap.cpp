#include <iostream>
#include <sstream>
#include <string>

#include <gtest/gtest.h>

#include <ltc/ltc.hpp>
#include <ltc/range.hpp>
#include <ltc/amap.hpp>

using namespace ltc;

class Test_amap : public ::testing::Test
{
};

TEST_F(Test_amap, default_construct)
{
    amap<std::string, int, 50> m1;
    ASSERT_EQ(m1.size(), 0);
    ASSERT_TRUE(m1.empty());
}

TEST_F(Test_amap, compare_construct)
{
    amap<std::string, int, 50> m1{ std::less<std::string>() };
    ASSERT_EQ(m1.size(), 0);
    ASSERT_TRUE(m1.empty());
}

TEST_F(Test_amap, range_construct)
{
    std::vector<std::pair<std::string, int>> v = { { "one", 1 }, { "two", 2 }, { "three", 3 } };
    amap<std::string, int, 50> m1{ v.begin(), v.end() };
    ASSERT_EQ(m1.size(), 3);
    ASSERT_FALSE(m1.empty());
}

TEST_F(Test_amap, initializer_list_construct)
{
    amap<std::string, int, 50> m = { { "3", 3 }, { "2", 2 }, { "1", 1 } };
    ASSERT_EQ(m.size(), 3);
    ASSERT_EQ(m.at("1"), 1);
    ASSERT_EQ(m.at("2"), 2);
    ASSERT_EQ(m.at("3"), 3);
}

TEST_F(Test_amap, copy_construct)
{
    const amap<std::string, int, 50> m = { { "3", 3 }, { "2", 2 }, { "1", 1 } };
    auto mc = amap<std::string, int, 50>(m);
    ASSERT_EQ(mc.size(), 3);
    ASSERT_EQ(m.size(), 3);
}

TEST_F(Test_amap, move_construct)
{
    amap<std::string, int, 50> m = { { "3", 3 }, { "2", 2 }, { "1", 1 } };
    auto mc = amap<std::string, int, 50>(std::move(m));
    ASSERT_EQ(mc.size(), 3);
    ASSERT_EQ(m.size(), 0);
}

TEST_F(Test_amap, assignment_copy)
{
    amap<std::string, int, 50> m = { { "3", 3 }, { "2", 2 }, { "1", 1 } };
    amap<std::string, int, 50> mc;
    mc = m;
    ASSERT_EQ(mc.size(), 3);
    ASSERT_EQ(m.size(), 3);
}

TEST_F(Test_amap, assignment_move)
{
    amap<std::string, int, 50> m = { { "3", 3 }, { "2", 2 }, { "1", 1 } };
    amap<std::string, int, 50> mc;
    mc = std::move(m);
    ASSERT_EQ(mc.size(), 3);
    ASSERT_EQ(m.size(), 0);
}

TEST_F(Test_amap, assignment_initializer_list)
{
    amap<std::string, int, 50> m;
    m = { { "3", 3 }, { "2", 2 }, { "1", 1 } };
    ASSERT_EQ(m.size(), 3);
}

TEST_F(Test_amap, at)
{
    amap<std::string, int, 50> m;
    ASSERT_THROW(m.at("one"), std::out_of_range);

    const amap<std::string, int, 50> cm;
    ASSERT_THROW(cm.at("one"), std::out_of_range);

    m["one"] = 1;
    ASSERT_EQ(m.at("one"), 1);
}

TEST_F(Test_amap, operator_square_brackets)
{
    amap<std::string, int, 50> m;
    m["2"] = 2;
    ASSERT_EQ(m["2"], 2);
    m["1"] = 1;
    ASSERT_EQ(m["1"], 1);
    m["3"] = 3;
    ASSERT_EQ(m["3"], 3);
    m["2"] = 0;
    ASSERT_EQ(m["2"], 0);

    std::string mkey = "4";
    m[std::move(mkey)] = 4;
    ASSERT_TRUE(mkey.empty());
    ASSERT_EQ(m["4"], 4);
}

TEST_F(Test_amap, iterators)
{
    amap<std::string, int, 50> m;
    ASSERT_EQ(m.begin(), m.end());
    m["1"] = 1;
    ASSERT_NE(m.begin(), m.end());

    m["3"] = 3;
    m["2"] = 2;

    int last = 0;
    for (auto it = m.begin(); it != m.end(); it++)
    {
        ASSERT_GT(it->second, last);
        last = it->second;
    }

    last = 4;
    for (auto it = m.rbegin(); it != m.rend(); it++)
    {
        ASSERT_LT(it->second, last);
        last = it->second;
    }
}

TEST_F(Test_amap, clear)
{
    amap<std::string, int, 50> m = { { "3", 3 }, { "2", 2 }, { "1", 1 } };
    ASSERT_EQ(m.size(), 3);
    m.clear();
    ASSERT_TRUE(m.empty());
}

TEST_F(Test_amap, find)
{
    amap<std::string, int, 50> m;
    const auto it_not_found = m.find("one");
    ASSERT_EQ(it_not_found, m.end());

    m["one"] = 1;
    const auto it_found = m.find("one");
    ASSERT_NE(it_found, m.end());
    ASSERT_EQ(it_found->second, 1);
}

TEST_F(Test_amap, insert)
{
    amap<std::string, int, 50> m;
    ASSERT_TRUE(m.insert(std::make_pair("one", 1)).second);
    ASSERT_FALSE(m.insert(std::make_pair("one", 1)).second);

    ASSERT_EQ(m.insert(m.begin(), std::make_pair("two", 2))->second, 2);
    ASSERT_EQ(m.insert(m.begin(), std::make_pair("two", 2))->second, 2);

    ASSERT_EQ(m.size(), 2);
}

TEST_F(Test_amap, insert_range)
{
    using map_t = amap<std::string, int, 50>;
    map_t m = { { "3", 3 }, { "2", 2 }, { "1", 1 } };
    const auto make_value = [](int i) { return map_t::value_type(std::to_string(i), i); };
    auto r = make_transform<map_t::value_type>(range<int>(1, 10), make_value);
    m.insert(r.begin(), r.end());
    ASSERT_EQ(m.size(), 9);
}

TEST_F(Test_amap, insert_initializer_list)
{
    using map_t = amap<std::string, int, 50>;
    map_t m = { { "3", 3 }, { "2", 2 }, { "1", 1 } };
    m.insert(
    { { "1", 1 }, { "2", 2 }, { "3", 3 }, { "4", 4 }, { "5", 5 }, { "6", 6 }, { "7", 7 }, { "8", 8 }, { "9", 9 } });
    ASSERT_EQ(m.size(), 9);
}

TEST_F(Test_amap, erase)
{
    using map_t = amap<std::string, int, 50>;
    map_t m = { { "1", 1 }, { "2", 2 }, { "3", 3 }, { "4", 4 }, { "5", 5 }, { "6", 6 }, { "7", 7 }, { "8", 8 }, { "9", 9 } };
    auto const it = m.erase(m.begin());
    ASSERT_EQ(it, m.begin());
    ASSERT_EQ(m.size(), 8);

    ASSERT_EQ(m.erase("5"), 1);
    ASSERT_EQ(m.size(), 7);

    auto const it2 = m.erase(m.begin(), m.begin() + 3);
    ASSERT_EQ(it2, m.begin());
    ASSERT_EQ(m.size(), 4);
}

TEST_F(Test_amap, swap)
{
    using map_t = amap<std::string, int, 50>;
    map_t m = { { "3", 3 }, { "2", 2 }, { "1", 1 } };
    map_t mc;

    ASSERT_EQ(m.size(), 3);
    ASSERT_EQ(mc.size(), 0);

    mc.swap(m);

    ASSERT_EQ(m.size(), 0);
    ASSERT_EQ(mc.size(), 3);
}

TEST_F(Test_amap, count)
{
    using map_t = amap<std::string, int, 50>;
    map_t m = { { "3", 3 }, { "2", 2 }, { "1", 1 } };
    ASSERT_EQ(m.count("2"), 1);
    ASSERT_EQ(m.count("4"), 0);
}

TEST_F(Test_amap, contains)
{
    using map_t = amap<std::string, int, 50>;
    map_t m = { { "3", 3 }, { "2", 2 }, { "1", 1 } };
    ASSERT_TRUE(m.contains("2"));
    ASSERT_FALSE(m.contains("4"));
}

TEST_F(Test_amap, equal_range)
{
    using map_t = amap<std::string, int, 50>;
    map_t m = { { "3", 3 }, { "2", 2 }, { "1", 1 } };
    auto p = m.equal_range("2");
    ASSERT_EQ(p.first, m.begin() + 1);
    ASSERT_EQ(p.second, m.begin() + 2);
}

TEST_F(Test_amap, lower_bound)
{
    using map_t = amap<std::string, int, 50>;
    map_t m = { { "3", 3 }, { "2", 2 }, { "1", 1 } };
    auto p = m.lower_bound("2");
    ASSERT_EQ(p, m.begin() + 1);
}

TEST_F(Test_amap, upper_bound)
{
    using map_t = amap<std::string, int, 50>;
    map_t m = { { "3", 3 }, { "2", 2 }, { "1", 1 } };
    auto p = m.upper_bound("2");
    ASSERT_EQ(p, m.begin() + 2);
}

TEST_F(Test_amap, stress_test)
{
    const int gsize = 1000000;
    using map_t = amap<int, int, gsize>;
    using kv_t = map_t::value_type;

    reverse_range<int> i(0, gsize);
    const auto make_value = [](int i) { return kv_t(i, i); };
    auto r = make_transform<kv_t>(i.begin(), i.end(), make_value);

    const auto m = std::make_unique<map_t>(r.begin(), r.end());

    ASSERT_EQ(m->size(), gsize);

    for (auto i = 1; i <= gsize; i++)
        ASSERT_EQ(m->at(i), i);
}
