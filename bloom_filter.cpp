
#include "bloom_filter.h"


// Constructore
Bloom_Filter::Bloom_Filter(uint32_t numHashes)
{
    // Parameterizable on the number of hashes
    m_numHashes = numHashes;
}






/*
insert()


*/
void Bloom_Filter::insert(const uint32_t &pid)
{

}




/*
query()


*/
float Bloom_Filter::query(const uint32_t &pid) const
{

}





/*
clear()


*/
inline void Bloom_Filter::clear()
{
    m_bloom_filter.reset()
}






