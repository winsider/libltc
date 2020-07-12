#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <vector>

namespace ltc
{

    template <class Key, class T, class Compare = std::less<Key>, class Allocator = std::allocator<std::pair<Key, T>>>
    class vmap
    {
    public:
        using key_type = Key;
        using mapped_type = T;
        using value_type = std::pair<Key, T>;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using key_compare = Compare;
        using storage_type = std::vector<value_type, Allocator>;
        using iterator = typename storage_type::iterator;
        using const_iterator = typename storage_type::const_iterator;
        using reverse_iterator = typename storage_type::reverse_iterator;
        using const_reverse_iterator = typename storage_type::const_reverse_iterator;
        using allocator_type = Allocator;

        class value_compare
        {
            key_compare m_key_comp;

        public:
            value_compare(const key_compare &key_comp) : m_key_comp(key_comp) {}

            bool operator()(const value_type &a, const value_type &b) const
            {
                return m_key_comp(a.first, b.first);
            }
        };

        // Construction
        vmap() : m_key_comp(key_compare()), m_value_comp(key_compare()), m_storage(Allocator()) {}

        explicit vmap(const Compare &comp, const Allocator &alloc = Allocator())
        : m_key_comp(comp), m_value_comp(comp), m_storage(Allocator())
        {
        }

        explicit vmap(const Allocator &alloc)
        : m_key_comp(key_compare()), m_value_comp(key_compare()), m_storage(alloc)
        {
        }

        vmap(const vmap &other)
        : m_key_comp(key_compare()), m_value_comp(key_compare()), m_storage(other.m_storage)
        {
        }
        vmap(const vmap &other, const Allocator &alloc)
        : m_key_comp(key_compare()), m_value_comp(key_compare()), m_storage(other.m_storage, alloc)
        {
        }

        vmap(vmap &&other)
        : m_key_comp(key_compare()), m_value_comp(key_compare()), m_storage(std::move(other.m_storage))
        {
        }

        vmap(vmap &&other, const Allocator &alloc)
        : m_key_comp(key_compare()), m_value_comp(key_compare()),
          m_storage(std::move(other.m_storage), alloc)
        {
        }

        vmap(std::initializer_list<value_type> init,
             const Compare &comp = Compare(),
             const Allocator &alloc = Allocator())
        : m_key_comp(comp), m_value_comp(comp), m_storage(std::move(init), alloc)
        {
            std::sort(m_storage.begin(), m_storage.end(), m_value_comp);
        }

        vmap(std::initializer_list<value_type> init, const Allocator &alloc)
        : m_key_comp(key_compare()), m_value_comp(key_compare()), m_storage(std::move(init), alloc)
        {
            std::sort(m_storage.begin(), m_storage.end(), m_value_comp);
        }

        template <class InputIt>
        vmap(InputIt first, InputIt last, const Compare &comp = Compare(), const Allocator &alloc = Allocator())
        : m_key_comp(comp), m_value_comp(comp), m_storage(first, last, alloc)
        {
            std::sort(m_storage.begin(), m_storage.end(), m_value_comp);
            m_storage.erase(std::unique(m_storage.begin(), m_storage.end()), m_storage.end());
        }

        template <class InputIt>
        vmap(InputIt first, InputIt last, const Allocator &alloc)
        : m_key_comp(key_compare()), m_value_comp(key_compare()), m_storage(first, last, alloc)
        {
            std::sort(m_storage.begin(), m_storage.end(), m_value_comp);
            m_storage.erase(std::unique(m_storage.begin(), m_storage.end()), m_storage.end());
        }

        allocator_type get_allocator() const noexcept { return m_storage.get_allocator(); }

        vmap &operator=(const vmap &other)
        {
            m_storage = other.m_storage;
            return *this;
        }

        vmap &operator=(vmap &&other)
        {
            m_storage = std::move(other.m_storage);
            return *this;
        }

        vmap &operator=(std::initializer_list<value_type> ilist)
        {
            m_storage = std::move(ilist);
            return *this;
        }

        // Element access
        T &at(const Key &key)
        {
            auto value = value_type(key, mapped_type());
            auto it = std::lower_bound(m_storage.begin(), m_storage.end(), value, m_value_comp);
            if (it != m_storage.end() && !m_key_comp(value.first, it->first)) return it->second;
            throw std::out_of_range("key");
        }
        const T &at(const Key &key) const
        {
            auto value = value_type(key, mapped_type());
            auto it = std::lower_bound(m_storage.begin(), m_storage.end(), value, m_value_comp);
            if (it != m_storage.end() && !m_key_comp(value.first, it->first)) return it->second;
            throw std::out_of_range("key");
        }

        T &operator[](const Key &key)
        {
            auto value = value_type(key, mapped_type());
            auto it = std::lower_bound(m_storage.begin(), m_storage.end(), value, m_value_comp);
            if (it != m_storage.end() && !m_key_comp(value.first, it->first)) return it->second;
            return m_storage.insert(it, std::move(value))->second;
        }

        T &operator[](Key &&key)
        {
            auto value = value_type(std::move(key), mapped_type());
            auto it = std::lower_bound(m_storage.begin(), m_storage.end(), value, m_value_comp);
            if (it != m_storage.end() && !m_key_comp(value.first, it->first)) return it->second;
            return m_storage.insert(it, std::move(value))->second;
        }

        // Iterators
        iterator begin() noexcept { return m_storage.begin(); }

        const_iterator begin() const noexcept { return m_storage.begin(); }

        const_iterator cbegin() const noexcept { return m_storage.cbegin(); }

        reverse_iterator rbegin() noexcept { return m_storage.rbegin(); }

        const_reverse_iterator rbegin() const noexcept { return m_storage.rbegin(); }

        const_reverse_iterator crbegin() const noexcept { return m_storage.crbegin(); }

        iterator end() noexcept { return m_storage.end(); }
        const_iterator end() const noexcept { return m_storage.end(); }
        const_iterator cend() const noexcept { return m_storage.cend(); }
        reverse_iterator rend() noexcept { return m_storage.rend(); }
        const_reverse_iterator rend() const noexcept { return m_storage.rend(); }
        const_reverse_iterator crend() const noexcept { return m_storage.crend(); }

        // Modifiers
        void clear() { m_storage.clear(); }

        std::pair<iterator, bool> insert(const value_type &value)
        {
            auto it = std::lower_bound(m_storage.begin(), m_storage.end(), value, m_value_comp);
            if (it != m_storage.end() && !m_key_comp(value.first, it->first))
                return std::make_pair(it, false);
            return std::make_pair(m_storage.insert(it, value), true);
        }

        std::pair<iterator, bool> insert(value_type &&value)
        {
            auto it = std::lower_bound(m_storage.begin(), m_storage.end(), value, m_value_comp);
            if (it != m_storage.end() && !m_key_comp(value.first, it->first))
                return std::make_pair(it, false);
            return std::make_pair(m_storage.insert(it, std::move(value)), true);
        }

        iterator insert(const_iterator hint, const value_type &value)
        {
            // TODO: Make use of hint
            auto it = std::lower_bound(m_storage.begin(), m_storage.end(), value, m_value_comp);
            if (it != m_storage.end() && !m_key_comp(value.first, it->first)) return it;
            return m_storage.insert(it, value);
        }

        iterator insert(const_iterator hint, value_type &&value)
        {
            // TODO: Make use of hint
            auto it = std::lower_bound(m_storage.begin(), m_storage.end(), value, m_value_comp);
            if (it != m_storage.end() && !m_key_comp(value.first, it->first)) return it;
            return m_storage.insert(it, std::move(value));
        }

        template <class InputIt> void insert(InputIt first, InputIt last)
        {
            const auto insertValue = [&](const auto &value) { this->insert(value); };
            std::for_each(first, last, insertValue);
        }

        void insert(std::initializer_list<value_type> ilist)
        {
            const auto insertValue = [&](auto &&value) { this->insert(std::move(value)); };
            std::for_each(ilist.begin(), ilist.end(), insertValue);
        }

        iterator erase(const_iterator pos) { return m_storage.erase(pos); }

        iterator erase(const_iterator first, const_iterator last)
        {
            return m_storage.erase(first, last);
        }

        size_type erase(const key_type &key)
        {
            const auto it = find(key);
            if (it == end()) return 0;
            m_storage.erase(it);
            return 1;
        }

        void swap(vmap &other) noexcept { m_storage.swap(other.m_storage); }

        // Capacity
        void reserve(size_type size) { m_storage.reserve(size); }
        bool empty() const { return m_storage.empty(); }
        size_type size() const { return m_storage.size(); }
        size_type max_size() const { return m_storage.max_size(); }

        // Lookup
        size_type count(const Key &key) const { return find(key) != end() ? 1 : 0; }

        bool contains(const Key &key) const { return count(key) == 1; }

        iterator find(const Key &key)
        {
            auto it = std::lower_bound(m_storage.begin(), m_storage.end(),
                                       value_type(key, mapped_type()), m_value_comp);
            if (it != m_storage.end() && !m_key_comp(key, it->first)) return it;
            return m_storage.end();
        }

        const_iterator find(const Key &key) const
        {
            auto it = std::lower_bound(m_storage.begin(), m_storage.end(),
                                       value_type(key, mapped_type()), m_value_comp);
            if (it != m_storage.end() && !m_key_comp(key, it->first)) return it;
            return m_storage.end();
        }

        std::pair<iterator, iterator> equal_range(const Key &key)
        {
            return std::equal_range(m_storage.begin(), m_storage.end(),
                                    value_type(key, mapped_type()), m_value_comp);
        }

        std::pair<const_iterator, const_iterator> equal_range(const Key &key) const
        {
            return std::equal_range(m_storage.begin(), m_storage.end(),
                                    value_type(key, mapped_type()), m_value_comp);
        }

        template <class K> std::pair<iterator, iterator> equal_range(const K &x)
        {
            return std::equal_range(m_storage.begin(), m_storage.end(), value_type(x, mapped_type()), m_value_comp);
        }

        template <class K> std::pair<const_iterator, const_iterator> equal_range(const K &x) const
        {
            return std::equal_range(m_storage.begin(), m_storage.end(), value_type(x, mapped_type()), m_value_comp);
        }

        iterator lower_bound(const Key &key)
        {
            return std::lower_bound(m_storage.begin(), m_storage.end(),
                                    value_type(key, mapped_type()), m_value_comp);
        }

        const_iterator lower_bound(const Key &key) const
        {
            return std::lower_bound(m_storage.begin(), m_storage.end(),
                                    value_type(key, mapped_type()), m_value_comp);
        }

        template <class K> iterator lower_bound(const K &x)
        {
            return std::lower_bound(m_storage.begin(), m_storage.end(), value_type(x, mapped_type()), m_value_comp);
        }

        template <class K> const_iterator lower_bound(const K &x) const
        {
            return std::lower_bound(m_storage.begin(), m_storage.end(), value_type(x, mapped_type()), m_value_comp);
        }

        iterator upper_bound( const Key& key )
        {
            return std::upper_bound(m_storage.begin(), m_storage.end(), value_type(key,
            mapped_type()), m_value_comp);
        }

        const_iterator upper_bound(const Key &key) const
        {
            return std::upper_bound(m_storage.begin(), m_storage.end(),
                                    value_type(key, mapped_type()), m_value_comp);
        }

        template <class K> iterator upper_bound(const K &x)
        {
            return std::upper_bound(m_storage.begin(), m_storage.end(), value_type(x, mapped_type()), m_value_comp);
        }

        template <class K> const_iterator upper_bound(const K &x) const
        {
            return std::upper_bound(m_storage.begin(), m_storage.end(), value_type(x, mapped_type()), m_value_comp);
        }

        // Observers
        key_compare key_comp() const { return m_key_comp; }
        value_compare value_comp() const { return m_value_comp; }

    protected:
        key_compare m_key_comp;
        value_compare m_value_comp;

    private:
        storage_type m_storage;
    };
}