#pragma once

#include <ltc/vmap_base.hpp>
#include <ltc/avector.hpp>

namespace ltc
{
    template<class Key, class T, size_t N, class Compare = std::less<Key>>
    class amap : public vmap_base<avector<std::pair<Key,T>, N>, Compare>
    {
        using storage_type = avector<std::pair<Key,T>, N>;
        using base_type = vmap_base<storage_type, Compare>;

    public:
        amap() : base_type() {}

        explicit amap(const Compare &comp)
        : base_type(comp)
        {
        }

        template <class InputIt>
        amap(InputIt first, InputIt last, const Compare &comp = Compare())
        : base_type(comp, storage_type(first, last))
        {
        }

    };
}