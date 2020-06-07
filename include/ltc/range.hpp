#pragma once

#include <stdexcept>

namespace ltc
{
    template <typename T> struct range final
    {
        class iterator final
        {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = T *;
            using reference = T &;

            iterator(const T &v) : m_value(v) {}
            const iterator &operator++()
            {
                ++m_value;
                return *this;
            }
            bool operator!=(const iterator &o) const { return o.m_value != m_value; }
            T operator*() const { return m_value; }

        private:
            T m_value;
        };

        range(const T &b, const T &e) : m_begin(b), m_end(e)
        {
            if (b > e) throw std::out_of_range("begin > end");
        }

        iterator begin() const { return m_begin; }
        iterator end() const { return m_end; }

        range() = delete;

    private:
        T m_begin, m_end;
    };

    template <typename T> struct reverse_range final
    {
        class iterator final
        {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = T *;
            using reference = T &;

            iterator(const T &v) : m_value(v) {}
            const iterator &operator++()
            {
                --m_value;
                return *this;
            }
            bool operator!=(const iterator &o) const { return o.m_value != m_value; }
            T operator*() const { return m_value; }

        private:
            T m_value;
        };

        reverse_range(const T &b, const T &e) : m_begin(e), m_end(b)
        {
            if (b > e) throw std::out_of_range("begin > end");
        }

        iterator begin() const { return m_begin; }

        iterator end() const { return m_end; }

        reverse_range() = delete;
        reverse_range(const reverse_range &) = delete;

    private:
        T m_begin, m_end;
    };

    template <typename T, typename InputIter, typename Conv> struct transform_range final
    {
        class iterator final
        {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = T *;
            using reference = T &;

            iterator(InputIter iter, Conv conv) : m_conv(conv), m_iter(iter) {}
            const iterator &operator++()
            {
                ++m_iter;
                return *this;
            }
            bool operator!=(const iterator &o) const { return o.m_iter != m_iter; }
            T operator*() const { return m_conv(*m_iter); }

        private:
            Conv m_conv;
            InputIter m_iter;
        };


        transform_range(InputIter first, InputIter last, Conv conv)
        : m_begin(first, conv), m_end(last, conv)
        {
        }

        iterator begin() const { return m_begin; }
        iterator end() const { return m_end; }

        transform_range() = delete;

    private:
        iterator m_begin, m_end;
    };

    template <typename T, typename InputIter, typename Conv>
    transform_range<T, InputIter, Conv> make_transform(InputIter first, InputIter last, Conv conv)
    {
        return transform_range<T, InputIter, Conv>(first, last, conv);
    }

    template <typename T, typename Range, typename Conv>
    transform_range<T, typename Range:: iterator, Conv> make_transform(Range range, Conv conv)
    {
        return make_transform<T>(range.begin(), range.end(), conv);
    }

}
