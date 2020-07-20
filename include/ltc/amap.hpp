#pragma once

#include <ltc/avector.hpp>
#include <ltc/vmap_base.hpp>

namespace ltc
{
    template <class Key, class T, size_t N, class Compare = std::less<Key>>
    class amap : public vmap_base<avector<std::pair<Key, T>, N>, Compare>
    {
        using storage_type = avector<std::pair<Key, T>, N>;
        using base_type = vmap_base<storage_type, Compare>;

    public:
        amap() : base_type() {}

        explicit amap(const Compare &comp) : base_type(comp) {}

        amap(std::initializer_list<typename base_type::value_type> init, const Compare &comp = Compare())
        : base_type(comp, storage_type(std::move(init)))
        {
        }

        amap(const amap &other) : base_type(other) {}

        amap(amap &&other) : base_type(std::move(other)) {}

        template <class InputIt>
        amap(InputIt first, InputIt last, const Compare &comp = Compare())
        : base_type(comp, storage_type(first, last))
        {
        }

        amap &operator=(const amap &other)
        {
            *(static_cast<base_type *>(this)) = other;
            return *this;
        }

        amap &operator=(amap &&other)
        {
            *(static_cast<base_type *>(this)) = std::move(other);
            return *this;
        }

        amap &operator=(std::initializer_list<typename base_type::value_type> ilist)
        {
            *(static_cast<base_type *>(this)) = std::move(ilist);
            return *this;
        }
    };
} // namespace ltc