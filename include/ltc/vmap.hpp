#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <vector>

#include <ltc/vmap_base.hpp>

namespace ltc
{

    template <class Key, class T, class Compare = std::less<Key>, class Allocator = std::allocator<std::pair<Key, T>>>
    class vmap : public vmap_base<std::vector<std::pair<Key, T>, Allocator>, Compare>
    {
    public:
        using storage_type = std::vector<std::pair<Key, T>, Allocator>;
        using base_type = vmap_base<storage_type, Compare>;
        using allocator_type = Allocator;

        // Construction
        vmap() : base_type() {}

        explicit vmap(const Compare &comp, const Allocator &alloc = Allocator())
        : base_type(comp, storage_type(alloc))
        {
        }

        explicit vmap(const Allocator &alloc) : base_type(storage_type(alloc)) {}

        vmap(const vmap &other) : base_type(other) {}

        vmap(const vmap &other, const Allocator &alloc)
        : base_type(storage_type(other.begin(), other.end(), alloc))
        {
        }

        vmap(vmap &&other) : base_type(std::move(other)) {}

        vmap(vmap &&other, const Allocator &alloc) : base_type(storage_type(alloc))
        {
            std::move(other.begin(), other.end(), std::back_insert_iterator<storage_type>(this->m_storage));
            other.clear();
        }

        vmap(std::initializer_list<typename base_type::value_type> init,
             const Compare &comp = Compare(),
             const Allocator &alloc = Allocator())
        : base_type(storage_type(std::move(init), alloc))
        {
        }

        vmap(std::initializer_list<typename base_type::value_type> init, const Allocator &alloc)
        : base_type(storage_type(std::move(init), alloc))
        {
        }

        template <class InputIt>
        vmap(InputIt first, InputIt last, const Compare &comp = Compare(), const Allocator &alloc = Allocator())
        : base_type(comp, storage_type(first, last, alloc))
        {
        }

        template <class InputIt>
        vmap(InputIt first, InputIt last, const Allocator &alloc)
        : base_type(storage_type(first, last, alloc))
        {
        }

        allocator_type get_allocator() const noexcept { return this->m_storage.get_allocator(); }

        vmap &operator=(const vmap &other)
        {
            *static_cast<base_type *>(this) = other;
            return *this;
        }

        vmap &operator=(vmap &&other)
        {
            *static_cast<base_type *>(this) = std::move(other);
            return *this;
        }

        vmap &operator=(std::initializer_list<typename base_type::value_type> ilist)
        {
            *static_cast<base_type *>(this) = std::move(ilist);
            return *this;
        }
    };
} // namespace ltc