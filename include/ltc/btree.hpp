#pragma once

#include <algorithm>
#include <cassert>
#include <memory>
#include <string>
#include <vector>

namespace ltc
{

    struct btree
    {
        using key_type = std::string;
        using key_compare = std::less<key_type>;
        using mapped_type = std::string;
        using value_type = std::pair<key_type,mapped_type>;
        using values_type = std::vector<value_type>;
        using node_iterator = values_type::iterator;
        using size_type = values_type::size_type;

        struct iterator;

        struct value_compare
        {
            value_compare(const key_compare &key_comp) : m_key_comp(key_comp) {}
            bool operator()(const value_type &a, const value_type &b) const
            {
                return m_key_comp(a.first, b.first);
            }
            const key_compare m_key_comp;
        };

        struct node
        {
            node(size_type order)
            {
                _values.reserve(order);
            }

            node_iterator begin() { return _values.begin(); }
            node_iterator end() { return _values.end(); }
            bool is_full() const { return _values.size() == _values.capacity(); }
            node_iterator insert(node_iterator it, const value_type &v) { return _values.insert(it, v); }

        private:
            values_type _values;            
        };

        struct iterator
        {
            using value_type = std::pair<node&,node_iterator>; 
            iterator(node &node, node_iterator it) : _node(node), _it(it) {}
            bool operator==(iterator const& other) const { return _it == other._it; }
            bool operator!=(iterator const& other) const { return _it != other._it; }  
            value_type operator*() const { return value_type(_node,_it); }

            iterator& operator++()
            {
                _it++;
                return *this;
            }

        private:
            std::vector<node*> _stack;
            node& _node;
            node_iterator _it; 
        };

        btree(size_type order) : _order(order), _root(order), _key_comp(key_compare()), _value_comp(key_compare()) 
        {
        }

        iterator begin() { return iterator(_root, _root.begin()); }
        iterator end() { return iterator(_root, _root.end()); }

        iterator find(const key_type &key)
        {
            auto it = std::lower_bound(_root.begin(), _root.end(),
                                       value_type(key, mapped_type()), _value_comp);
            if (it != _root.end() && !_key_comp(key, it->first)) return iterator(_root, it);
            return end();
        }

        mapped_type const& at(key_type const& key) const
        {
            throw std::out_of_range("key");
        }

        std::pair<iterator, bool> insert(const value_type &value)
        {
            auto it = find(value.first);
            if ((*it).first.is_full())
            {
                // split node
                
                throw std::runtime_error("node full");
            }
            return std::make_pair(iterator(_root, _root.insert((*it).second, value)), true);
        }

    private:
        node _root;
        size_type _order;
        key_compare _key_comp;
        value_compare _value_comp;
    };

} // namespace ltc