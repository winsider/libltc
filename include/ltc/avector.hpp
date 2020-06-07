#pragma once

#include <algorithm>
#include <array>
#include <assert.h>
#include <cstddef>
#include <functional>
#include <initializer_list>

namespace ltc
{

    template <typename T, size_t N> class avector
    {
    public:
        using size_type = typename std::array<T, N>::size_type;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using reference = value_type &;
        using const_reference = const value_type &;
        using storage_type = typename std::array<T, N>;
        using iterator = typename storage_type::iterator;
        using const_iterator = typename storage_type::const_iterator;

        avector() { m_end = m_storage.begin(); }
        avector(std::initializer_list<T> init)
        {
            m_end = std::move(init.begin(), init.end(), m_storage.begin());
        }

        // Iterators
        iterator begin() { return m_storage.begin(); }
        iterator end() { return m_end; }

        // Capacity
        bool empty() const noexcept { return m_storage.begin() == m_end; };
        size_type size() const noexcept { return m_end - m_storage.begin(); }
        size_type max_size() const noexcept { return m_storage.max_size(); }
        void reserve(size_type new_cap)
        {
            if (new_cap > m_storage.max_size()) throw std::length_error("new_cap");
        }
        size_type capacity() const noexcept { return N; }
        void shrink_to_fit() { /* no op */}

        // Element access
        reference at(size_type pos)
        {
            return m_storage[pos];
        }

        const_reference at(size_type pos) const
        {
            return m_storage[pos];
        }

        // Modifiers
        void clear() noexcept
        {
            m_storage.fill(T());
            m_end = m_storage.begin();
        }

        iterator insert(const_iterator pos, const T &value)
        {
            assert(pos >= m_storage.begin() && pos <= m_end);
            if (size() == capacity()) throw std::length_error("insert");
            iterator it = m_storage.begin() + (pos - m_storage.begin());
            std::move_backward(it, m_end, ++m_end);
            *it = value;
            return it;
        }

        iterator insert(const_iterator pos, T &&value)
        {
            assert(pos >= m_storage.begin() && pos <= m_end);
            if (size() == capacity()) throw std::length_error("insert");
            iterator it = m_storage.begin() + (pos - m_storage.begin());
            std::move_backward(it, m_end, ++m_end);
            *it = std::move(value);
            return it;
        }

        iterator insert(const_iterator pos, size_type count, const T &value)
        {
            assert(pos >= m_storage.begin() && pos <= m_end);
            if (size() + count > capacity()) throw std::length_error("insert");
            iterator it = m_storage.begin() + (pos - m_storage.begin());
            std::move_backward(it, m_end, m_end += count);
            std::fill(it, it + count, value);
            return it;
        }

    private:
        iterator m_end;
        storage_type m_storage;
    };
}