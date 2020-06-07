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
        using reverse_iterator = typename storage_type::reverse_iterator;
        using const_reverse_iterator = typename storage_type::const_reverse_iterator;

        avector() { m_end = m_storage.begin(); }
        avector(std::initializer_list<T> init)
        {
            m_end = std::move(init.begin(), init.end(), m_storage.begin());
        }

        // Iterators
        iterator begin() noexcept { return m_storage.begin(); }
        const_iterator begin() const noexcept { return m_storage.begin(); }
        iterator end() noexcept { return m_end; }
        const_iterator end() const noexcept { return m_end; }
        reverse_iterator rbegin() noexcept { return std::make_reverse_iterator(m_end); }
        const_reverse_iterator rbegin() const noexcept { return std::make_reverse_iterator(m_end); }
        reverse_iterator rend() noexcept { return std::make_reverse_iterator(m_storage.begin()); }
        const_reverse_iterator rend() const noexcept
        {
            return std::make_reverse_iterator(m_storage.begin());
        }

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
        reference at(size_type pos) { return m_storage[pos]; }
        const_reference at(size_type pos) const { return m_storage[pos]; }
        reference operator[](size_type pos) { return m_storage[pos]; }
        const_reference operator[](size_type pos) const { return m_storage[pos]; }

        reference front() { return m_storage.front(); }
        const_reference front() const { return m_storage.front(); }
        reference back() { return *rbegin(); }
        const_reference back() const { return *rbegin(); }

        // Modifiers
        void clear() noexcept
        {
            m_storage.fill(T());
            m_end = m_storage.begin();
        }

        template <class InputIt> iterator insert(const_iterator pos, InputIt first, InputIt last)
        {
            assert(pos >= m_storage.begin() && pos <= m_end);
            const auto count = last - first;
            if (size() + count > N) throw std::length_error("insert");
            iterator it = m_storage.begin() + (pos - m_storage.begin());
            std::move_backward(it, m_end, m_end + count);
            std::copy(first, last, it);
            m_end += count;
            return it;
        }

        iterator insert(const_iterator pos, const T &value)
        {
            assert(pos >= m_storage.begin() && pos <= m_end);
            if (size() == N) throw std::length_error("insert");
            iterator it = m_storage.begin() + (pos - m_storage.begin());
            std::move_backward(it, m_end, m_end + 1);
            *it = value;
            ++m_end;
            return it;
        }

        iterator insert(const_iterator pos, T &&value)
        {
            assert(pos >= m_storage.begin() && pos <= m_end);
            if (size() == N) throw std::length_error("insert");
            iterator it = m_storage.begin() + (pos - m_storage.begin());
            std::move_backward(it, m_end, m_end + 1);
            *it = std::move(value);
            ++m_end;
            return it;
        }

        iterator insert(const_iterator pos, size_type count, const T &value)
        {
            assert(pos >= m_storage.begin() && pos <= m_end);
            if (size() + count > N) throw std::length_error("insert");
            iterator it = m_storage.begin() + (pos - m_storage.begin());
            std::move_backward(it, m_end, m_end + count);
            std::fill(it, it + count, value);
            m_end += count;
            return it;
        }

        iterator insert(const_iterator pos, std::initializer_list<T> ilist)
        {
            return insert(pos, ilist.begin(), ilist.end());
        }

        template <class... Args> iterator emplace(const_iterator pos, Args &&... args)
        {
            assert(pos >= m_storage.begin() && pos <= m_end);
            iterator it = m_storage.begin() + (pos - m_storage.begin());
            new (it) T(args...);
            return it;
        }

        template <class... Args> reference emplace_back(Args &&... args)
        {
            if (size() == N) throw std::length_error("emplace_back");
            iterator it = m_end++;
            new (it) T(args...);
            return *it;
        }

    private:
        iterator m_end;
        storage_type m_storage;
    };
}