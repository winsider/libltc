#include <cmath>
#include <functional>
#include <vector>

namespace ltc
{
    // https://hur.st/bloomfilter/?n=40000000000&p=1.0E-6&m=&k=30
    // https://llimllib.github.io/bloomfilter-tutorial/
    // https://findingprotopia.org/posts/how-to-write-a-bloom-filter-cpp/
    
    class bloom_calculator final
    {
    public:
        bloom_calculator() : m_items(4000), m_probability(0.0000001), m_bits(134191), m_hashes(23)
        {
        }

        uint64_t items() const { return m_items; }
        void items(uint64_t i)
        {
            m_items = i;
            calc_p();
            calc_m();
            calc_k();
        }

        double probability() const { return m_probability; }
        void probability(double p)
        {
            m_probability = p;
            calc_n();
            calc_m();
            calc_k();
        }

        size_t bits() const { return m_bits; }
        void bits(size_t bits)
        {
            m_bits = bits;
            calc_n();
            calc_p();
            calc_k();
        }

        static inline uint64_t calc_items(size_t bits, uint8_t hashes, double probability)
        {
            return std::ceil(bits / (-hashes / std::log(1 - std::exp(std::log(probability) / hashes))));
        }

        static inline double calc_probability(size_t bits, uint8_t hashes, uint64_t items)
        {
            return std::pow(1 - std::exp(-hashes / (bits / items)), hashes);
        }

        static inline size_t calc_bits(uint64_t items, double probability)
        {
            return std::ceil((items * std::log(probability)) / std::log(1 / std::pow(2, std::log(2))));
        }

        static inline uint8_t calc_hashes(size_t bits, uint64_t items)
        {
            return std::round((bits / items) * std::log(2));
        }

    private:
        void calc_n() { m_items = calc_items(m_bits, m_hashes, m_probability); }
        void calc_p() { m_probability = calc_probability(m_bits, m_hashes, m_items); }
        void calc_m() { m_bits = calc_bits(m_items, m_probability); }
        void calc_k() { m_hashes = calc_hashes(m_bits, m_items); }

        uint64_t m_items;
        double m_probability;
        size_t m_bits;
        uint8_t m_hashes;
    };

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
        size_t m_count{ 0 };
        const uint8_t m_num_hashes;
    };

} // namespace ltc
