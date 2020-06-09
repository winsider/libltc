#include <iostream>
#include <sstream>
#include <string>

#include <gtest/gtest.h>

#include <ltc/range.hpp>
#include <ltc/vset.hpp>

using namespace ltc;

class Test_vset : public ::testing::Test
{
};

TEST_F(Test_vset, default_construct)
{
    vset<std::string> m1;
    ASSERT_EQ(m1.size(), 0);
    ASSERT_TRUE(m1.empty());
}

TEST_F(Test_vset, compare_construct)
{
    vset<std::string> m1{ std::less<std::string>() };
    ASSERT_EQ(m1.size(), 0);
    ASSERT_TRUE(m1.empty());
}


TEST_F(Test_vset, allocator_construct)
{
    vset<std::string> m1{ std::allocator<std::string>() };
    ASSERT_EQ(m1.size(), 0);
    ASSERT_TRUE(m1.empty());
}

TEST_F(Test_vset, range_construct)
{
    std::vector<std::string> v = { "one", "two", "three" };
    vset<std::string> m1{ v.begin(), v.end() };
    ASSERT_EQ(m1.size(), 3);
    ASSERT_FALSE(m1.empty());
}

TEST_F(Test_vset, allocator_range_construct)
{
    std::vector<std::string> v = { "one", "two", "three" };
    vset<std::string> m1{ v.begin(), v.end(), std::allocator<std::string>() };
    ASSERT_EQ(m1.size(), 3);
    ASSERT_FALSE(m1.empty());
}

TEST_F(Test_vset, copy_construct)
{
    const vset<std::string> m = { "3", "2", "1" };
    auto mc = vset<std::string>(m);
    ASSERT_EQ(mc.size(), 3);
    ASSERT_EQ(m.size(), 3);
}

TEST_F(Test_vset, copy_alloc_construct)
{
    const vset<std::string> m = { "3", "2", "1" };
    auto mc = vset<std::string>(m, std::allocator<std::string>());
    ASSERT_EQ(mc.size(), 3);
    ASSERT_EQ(m.size(), 3);
}

TEST_F(Test_vset, move_construct)
{
    vset<std::string> m = { "3", "2", "1" };
    auto mc = vset<std::string>(std::move(m));
    ASSERT_EQ(mc.size(), 3);
    ASSERT_EQ(m.size(), 0);
}

TEST_F(Test_vset, move_alloc_construct)
{
    vset<std::string> m = { "3", "2", "1" };
    auto mc = vset<std::string>(std::move(m), std::allocator<std::string>());
    ASSERT_EQ(mc.size(), 3);
    ASSERT_EQ(m.size(), 0);
}

TEST_F(Test_vset, initializer_list_construct)
{
    vset<std::string> m = { "3", "2", "1" };
    ASSERT_EQ(m.size(), 3);
    ASSERT_TRUE(m.contains("1"));
    ASSERT_TRUE(m.contains("2"));
    ASSERT_TRUE(m.contains("3"));
}

TEST_F(Test_vset, assignment)
{
    vset<std::string> m = { "3", "2", "1" };
    vset<std::string> mc;
    mc = m;
    ASSERT_EQ(mc.size(), 3);
    ASSERT_EQ(m.size(), 3);
}

TEST_F(Test_vset, assignment_move)
{
    vset<std::string> m = { "3", "2", "1" };
    vset<std::string> mc;
    mc = std::move(m);
    ASSERT_EQ(mc.size(), 3);
    ASSERT_EQ(m.size(), 0);
}

TEST_F(Test_vset, assignment_initializer_list)
{
    vset<std::string, int> m;
    m = { "3", "2", "1" };
    ASSERT_EQ(m.size(), 3);
}

TEST_F(Test_vset, get_allocator)
{
    const auto alloc = std::allocator<std::string>();
    vset<std::string> m(alloc);
    ASSERT_EQ(m.get_allocator(), alloc);
}

TEST_F(Test_vset, iterators)
{
    vset<int> m;
    ASSERT_EQ(m.begin(), m.end());
    m.insert(1);
    ASSERT_NE(m.begin(), m.end());

    m.insert(3);
    m.insert(2);

    int last = 0;
    for (auto it = m.begin(); it != m.end(); it++)
    {
        ASSERT_GT(*it, last);
        last = *it;
    }

    last = 4;
    for (auto it = m.rbegin(); it != m.rend(); it++)
    {
        ASSERT_LT(*it, last);
        last = *it;
    }
}

TEST_F(Test_vset, clear)
{
    vset<std::string> m = { "3", "2", "1" };
    ASSERT_EQ(m.size(), 3);
    m.clear();
    ASSERT_TRUE(m.empty());
}

TEST_F(Test_vset, find)
{
    vset<std::string> m;
    const auto it_not_found = m.find("one");
    ASSERT_EQ(it_not_found, m.end());

    m.insert("one");
    const auto it_found = m.find("one");
    ASSERT_NE(it_found, m.end());
    ASSERT_EQ(*it_found, "one");
}

TEST_F(Test_vset, insert)
{
    vset<std::string> m;
    ASSERT_TRUE(m.insert("one").second);
    ASSERT_FALSE(m.insert("one").second);

    ASSERT_EQ(*(m.insert(m.begin(), "two")), "two");
    ASSERT_EQ(*(m.insert(m.begin(), "two")), "two");

    ASSERT_EQ(m.size(), 2);
}

TEST_F(Test_vset, insert_range)
{
    using set_t = vset<std::string>;
    set_t m = { "3", "2", "1" };
    const auto make_value = [](int i) { return std::to_string(i); };
    auto r = make_transform<set_t::value_type>(range<int>(1, 10), make_value);
    m.insert(r.begin(), r.end());
    ASSERT_EQ(m.size(), 9);
}

TEST_F(Test_vset, insert_initializer_list)
{
    using set_t = vset<std::string>;
    set_t m = { "3", "2", "1" };
    m.insert({ "1", "2", "3", "4", "5", "6", "7", "8", "9" });
    ASSERT_EQ(m.size(), 9);
}

TEST_F(Test_vset, erase)
{
    using set_t = vset<std::string>;
    set_t m = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
    auto const it = m.erase(m.begin());
    ASSERT_EQ(it, m.begin());
    ASSERT_EQ(m.size(), 8);

    ASSERT_EQ(m.erase("5"), 1);
    ASSERT_EQ(m.size(), 7);

    const auto it2 = m.erase(m.begin(), m.begin() + 3);
    ASSERT_EQ(it2, m.begin());
    ASSERT_EQ(m.size(), 4);
}

TEST_F(Test_vset, swap)
{
    using set_t = vset<std::string>;
    set_t m = { "3", "2", "1" };
    set_t mc;

    ASSERT_EQ(m.size(), 3);
    ASSERT_EQ(mc.size(), 0);

    mc.swap(m);

    ASSERT_EQ(m.size(), 0);
    ASSERT_EQ(mc.size(), 3);
}

TEST_F(Test_vset, count)
{
    using set_t = vset<std::string>;
    set_t m = { "3", "2", "1" };
    ASSERT_EQ(m.count("2"), 1);
    ASSERT_EQ(m.count("4"), 0);
}

TEST_F(Test_vset, contains)
{
    using set_t = vset<std::string>;
    set_t m = { "3", "2", "1" };
    ASSERT_TRUE(m.contains("2"));
    ASSERT_FALSE(m.contains("4"));
}

TEST_F(Test_vset, equal_range)
{
    using set_t = vset<std::string>;
    set_t m = { "3", "2", "1" };
    auto p = m.equal_range("2");
    ASSERT_EQ(p.first, m.begin() + 1);
    ASSERT_EQ(p.second, m.begin() + 2);
}

TEST_F(Test_vset, lower_bound)
{
    using set_t = vset<std::string>;
    set_t m = { "3", "2", "1" };
    auto p = m.lower_bound("2");
    ASSERT_EQ(p, m.begin() + 1);
}

TEST_F(Test_vset, upper_bound)
{
    using set_t = vset<std::string>;
    set_t m = { "3", "2", "1" };
    auto p = m.upper_bound("2");
    ASSERT_EQ(p, m.begin() + 2);
}

TEST_F(Test_vset, stress_test)
{
    using set_t = vset<int>;

    const int gsize = 1000000;
    reverse_range<int> r(0, gsize);

    vset<int> m(r.begin(), r.end());

    ASSERT_EQ(m.size(), gsize);

    for (auto i = 1; i <= gsize; i++)
        ASSERT_TRUE(m.contains(i));
}
