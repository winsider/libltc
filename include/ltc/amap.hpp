#pragma once

#include <ltc/vmap_base.hpp>
#include <ltc/avector.hpp>

namespace ltc
{
    template<class Key, class T, size_t N, class Compare = std::less<Key>>
    class amap : public vmap_base<avector<std::pair<Key,T>, N>, Compare>
    {
    public:

    };
}