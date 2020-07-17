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

// TEST_F(Test_vmap, allocator_range_construct)
// {
//     std::vector<std::pair<std::string, int>> v = { { "one", 1 }, { "two", 2 }, { "three", 3 } };
//     vmap<std::string, int> m1{ v.begin(), v.end(), std::allocator<std::pair<std::string, int>>() };
//     ASSERT_EQ(m1.size(), 3);
//     ASSERT_FALSE(m1.empty());
// }

// TEST_F(Test_vmap, copy_construct)
// {
//     const vmap<std::string, int> m = { { "3", 3 }, { "2", 2 }, { "1", 1 } };
//     auto mc = vmap<std::string, int>(m);
//     ASSERT_EQ(mc.size(), 3);
//     ASSERT_EQ(m.size(), 3);
// }

// TEST_F(Test_vmap, copy_alloc_construct)
// {
//     const vmap<std::string, int> m = { { "3", 3 }, { "2", 2 }, { "1", 1 } };
//     auto mc = vmap<std::string, int>(m, std::allocator<std::pair<std::string, int>>());
//     ASSERT_EQ(mc.size(), 3);
//     ASSERT_EQ(m.size(), 3);
// }

// TEST_F(Test_vmap, move_construct)
// {
//     vmap<std::string, int> m = { { "3", 3 }, { "2", 2 }, { "1", 1 } };
//     auto mc = vmap<std::string, int>(std::move(m));
//     ASSERT_EQ(mc.size(), 3);
//     ASSERT_EQ(m.size(), 0);
// }

// TEST_F(Test_vmap, move_alloc_construct)
// {
//     vmap<std::string, int> m = { { "3", 3 }, { "2", 2 }, { "1", 1 } };
//     auto mc = vmap<std::string, int>(std::move(m), std::allocator<std::pair<std::string, int>>());
//     ASSERT_EQ(mc.size(), 3);
//     ASSERT_EQ(m.size(), 0);
// }

// TEST_F(Test_vmap, initializer_list_construct)
// {
//     vmap<std::string, int> m = { { "3", 3 }, { "2", 2 }, { "1", 1 } };
//     ASSERT_EQ(m.size(), 3);
//     ASSERT_EQ(m.at("1"), 1);
//     ASSERT_EQ(m.at("2"), 2);
//     ASSERT_EQ(m.at("3"), 3);
// }

// TEST_F(Test_vmap, assignment)
// {
//     vmap<std::string, int> m = { { "3", 3 }, { "2", 2 }, { "1", 1 } };
//     vmap<std::string, int> mc;
//     mc = m;
//     ASSERT_EQ(mc.size(), 3);
//     ASSERT_EQ(m.size(), 3);
// }

// TEST_F(Test_vmap, assignment_move)
// {
//     vmap<std::string, int> m = { { "3", 3 }, { "2", 2 }, { "1", 1 } };
//     vmap<std::string, int> mc;
//     mc = std::move(m);
//     ASSERT_EQ(mc.size(), 3);
//     ASSERT_EQ(m.size(), 0);
// }

// TEST_F(Test_vmap, assignment_initializer_list)
// {
//     vmap<std::string, int> m;
//     m = { { "3", 3 }, { "2", 2 }, { "1", 1 } };
//     ASSERT_EQ(m.size(), 3);
// }

// TEST_F(Test_vmap, get_allocator)
// {
//     const auto alloc = std::allocator<std::pair<std::string, int>>();
//     vmap<std::string, int> m(alloc);
//     ASSERT_EQ(m.get_allocator(), alloc);
// }

// TEST_F(Test_vmap, at)
// {
//     vmap<std::string, int> m;
//     ASSERT_THROW(m.at("one"), std::out_of_range);

//     const vmap<std::string, int> cm;
//     ASSERT_THROW(cm.at("one"), std::out_of_range);

//     m["one"] = 1;
//     ASSERT_EQ(m.at("one"), 1);
// }

// TEST_F(Test_vmap, operator_square_brackets)
// {
//     vmap<std::string, int> m;
//     m["2"] = 2;
//     ASSERT_EQ(m["2"], 2);
//     m["1"] = 1;
//     ASSERT_EQ(m["1"], 1);
//     m["3"] = 3;
//     ASSERT_EQ(m["3"], 3);
//     m["2"] = 0;
//     ASSERT_EQ(m["2"], 0);

//     std::string mkey = "4";
//     m[std::move(mkey)] = 4;
//     ASSERT_TRUE(mkey.empty());
//     ASSERT_EQ(m["4"], 4);
// }

// TEST_F(Test_vmap, iterators)
// {
//     vmap<std::string, int> m;
//     ASSERT_EQ(m.begin(), m.end());
//     m["1"] = 1;
//     ASSERT_NE(m.begin(), m.end());

//     m["3"] = 3;
//     m["2"] = 2;

//     int last = 0;
//     for (auto it = m.begin(); it != m.end(); it++)
//     {
//         ASSERT_GT(it->second, last);
//         last = it->second;
//     }

//     last = 4;
//     for (auto it = m.rbegin(); it != m.rend(); it++)
//     {
//         ASSERT_LT(it->second, last);
//         last = it->second;
//     }
// }

// TEST_F(Test_vmap, clear)
// {
//     vmap<std::string, int> m = { { "3", 3 }, { "2", 2 }, { "1", 1 } };
//     ASSERT_EQ(m.size(), 3);
//     m.clear();
//     ASSERT_TRUE(m.empty());
// }

// TEST_F(Test_vmap, find)
// {
//     vmap<std::string, int> m;
//     const auto it_not_found = m.find("one");
//     ASSERT_EQ(it_not_found, m.end());

//     m["one"] = 1;
//     const auto it_found = m.find("one");
//     ASSERT_NE(it_found, m.end());
//     ASSERT_EQ(it_found->second, 1);
// }

// TEST_F(Test_vmap, insert)
// {
//     vmap<std::string, int> m;
//     ASSERT_TRUE(m.insert(std::make_pair("one", 1)).second);
//     ASSERT_FALSE(m.insert(std::make_pair("one", 1)).second);

//     ASSERT_EQ(m.insert(m.begin(), std::make_pair("two", 2))->second, 2);
//     ASSERT_EQ(m.insert(m.begin(), std::make_pair("two", 2))->second, 2);

//     ASSERT_EQ(m.size(), 2);
// }

// TEST_F(Test_vmap, insert_range)
// {
//     using map_t = vmap<std::string, int>;
//     map_t m = { { "3", 3 }, { "2", 2 }, { "1", 1 } };
//     const auto make_value = [](int i) { return map_t::value_type(std::to_string(i), i); };
//     auto r = make_transform<map_t::value_type>(range<int>(1, 10), make_value);
//     m.insert(r.begin(), r.end());
//     ASSERT_EQ(m.size(), 9);
// }

// TEST_F(Test_vmap, insert_initializer_list)
// {
//     using map_t = vmap<std::string, int>;
//     map_t m = { { "3", 3 }, { "2", 2 }, { "1", 1 } };
//     m.insert(
//     { { "1", 1 }, { "2", 2 }, { "3", 3 }, { "4", 4 }, { "5", 5 }, { "6", 6 }, { "7", 7 }, { "8", 8 }, { "9", 9 } });
//     ASSERT_EQ(m.size(), 9);
// }

// TEST_F(Test_vmap, erase)
// {
//     using map_t = vmap<std::string, int>;
//     map_t m = { { "1", 1 }, { "2", 2 }, { "3", 3 }, { "4", 4 }, { "5", 5 }, { "6", 6 }, { "7", 7 }, { "8", 8 }, { "9", 9 } };
//     auto const it = m.erase(m.begin());
//     ASSERT_EQ(it, m.begin());
//     ASSERT_EQ(m.size(), 8);

//     ASSERT_EQ(m.erase("5"), 1);
//     ASSERT_EQ(m.size(), 7);

//     auto const it2 = m.erase(m.begin(), m.begin() + 3);
//     ASSERT_EQ(it2, m.begin());
//     ASSERT_EQ(m.size(), 4);
// }

// TEST_F(Test_vmap, swap)
// {
//     using map_t = vmap<std::string, int>;
//     map_t m = { { "3", 3 }, { "2", 2 }, { "1", 1 } };
//     map_t mc;

//     ASSERT_EQ(m.size(), 3);
//     ASSERT_EQ(mc.size(), 0);

//     mc.swap(m);

//     ASSERT_EQ(m.size(), 0);
//     ASSERT_EQ(mc.size(), 3);
// }

// TEST_F(Test_vmap, count)
// {
//     using map_t = vmap<std::string, int>;
//     map_t m = { { "3", 3 }, { "2", 2 }, { "1", 1 } };
//     ASSERT_EQ(m.count("2"), 1);
//     ASSERT_EQ(m.count("4"), 0);
// }

// TEST_F(Test_vmap, contains)
// {
//     using map_t = vmap<std::string, int>;
//     map_t m = { { "3", 3 }, { "2", 2 }, { "1", 1 } };
//     ASSERT_TRUE(m.contains("2"));
//     ASSERT_FALSE(m.contains("4"));
// }

// TEST_F(Test_vmap, equal_range)
// {
//     using map_t = vmap<std::string, int>;
//     map_t m = { { "3", 3 }, { "2", 2 }, { "1", 1 } };
//     auto p = m.equal_range("2");
//     ASSERT_EQ(p.first, m.begin() + 1);
//     ASSERT_EQ(p.second, m.begin() + 2);
// }

// TEST_F(Test_vmap, lower_bound)
// {
//     using map_t = vmap<std::string, int>;
//     map_t m = { { "3", 3 }, { "2", 2 }, { "1", 1 } };
//     auto p = m.lower_bound("2");
//     ASSERT_EQ(p, m.begin() + 1);
// }

// TEST_F(Test_vmap, upper_bound)
// {
//     using map_t = vmap<std::string, int>;
//     map_t m = { { "3", 3 }, { "2", 2 }, { "1", 1 } };
//     auto p = m.upper_bound("2");
//     ASSERT_EQ(p, m.begin() + 2);
// }

// TEST_F(Test_vmap, stress_test)
// {
//     using map_t = vmap<int, int>;
//     using kv_t = map_t::value_type;

//     const int gsize = 1000000;
//     reverse_range<int> i(0, gsize);
//     const auto make_value = [](int i) { return kv_t(i, i); };
//     auto r = make_transform<kv_t>(i.begin(), i.end(), make_value);

//     vmap<int, int> m(r.begin(), r.end());

//     ASSERT_EQ(m.size(), gsize);

//     for (auto i = 1; i <= gsize; i++)
//         ASSERT_EQ(m.at(i), i);
// }


// #include <map>
// TEST_F(Test_vmap, mapstress_test)
// {
//     const int gsize = 1000000;
//     std::map<int, int> m;
//     for (auto i = gsize; i > 0; --i)
//         m[i] = i;

//     ASSERT_EQ(m.size(), gsize);

//     for (auto i = 1; i <= gsize; i++)
//         ASSERT_EQ(m.at(i), i);
// }
