#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

#include <cstdint>
#include <bitset>

template <unsigned int SIZE>
class Bloom_Filter
{
  public:
    //Constructor
    Bloom_Filter(uint32_t hashes); 

    void insert(const uint32_t &pid);
    float query(const uint32_t &pid) const;    
    void clear();


  private:
    // Bloom parameters
    const uint32_t m_bloomSize = SIZE;
    uint32_t m_numHashes;
    uint32_t m_numInsertions = 0;

    // Actual Bloom filter
    // could be implemted as a bitset if size known at compile time
    std::bitset<SIZE> m_bloom_filter;

    // Bloom structure
};

#endif
