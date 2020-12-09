#include <functional>
#include <vector>

namespace ltc
{
    template <typename Key, typename Hash = std::hash<Key>> class bloom_filter
    {
    public:
        bloom_filter(size_t size, uint8_t num_hashes) : m_bits(size), m_num_hashes(num_hashes) {}

        void add(const Key &key) 
        {
            const Hash hasher{};
            const auto size = m_bits.size();
            auto hash = hasher(key);
            for (auto n = 0; n < m_num_hashes; ++n, hash = hash ^ ((hash + n) << 1))
            {
                m_bits[hash % size] = true;                
            }
            ++m_count;
        }

        bool try_add(const Key &key) 
        {
            const Hash hasher{};
            const auto size = m_bits.size();
            auto hash = hasher(key);
            bool added = false;
            for (auto n = 0; n < m_num_hashes; ++n, hash = hash ^ ((hash + n) << 1))
            {
                const auto idx = hash % size;
                if (!m_bits[idx])
                {
                    added = true;
                } 
                m_bits[idx] = true;                
            }
            if (added)
            {
                ++m_count;
            }   
            return added;         
        }

        bool possibly_contains(const Key &key) const 
        { 
            const Hash hasher{};
            const auto size = m_bits.size();
            auto hash = hasher(key);
            for (auto n = 0; n < m_num_hashes; ++n, hash = hash ^ ((hash + n) << 1))
            {
                if (!m_bits[hash % size])
                {
                    return false;
                }                
            }
            return true;
        }

        size_t size() const { return m_bits.size(); }
        size_t count() const { return m_count; }

    private:
        std::vector<bool> m_bits;
        size_t m_count{0};
        const uint8_t m_num_hashes;
    };

} // namespace ltc
