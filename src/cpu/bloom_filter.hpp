#ifndef BLOOM_FILTER_HPP
#define BLOOM_FILTER_HPP


#include "MurmurHash3.hpp"

#include "common.hpp"


class bloom_filter
{
public:
    bloom_filter();
    bloom_filter(uint32_t size, uint32_t num_hashes);

    void add(uint32_t key);
    bool contains(uint32_t key);

    uint32_t get_size();
    uint32_t get_count();
    uint32_t get_num_hashes();    


private:
    uint32_t m_size;        // num bits
    uint32_t m_indices;     // num of 32-bit indices in filter
    uint32_t m_count;       // number of keys inserted
    uint32_t m_num_hashes;  // number of hashes to use

    uint32_t m_bloom_filter[BLOOM_FILTER_SIZE / 32];        // bit vector comprised of 32 bits?
};

#endif
