// bloom_filter.cpp


#include "bloom_filter.hpp"


/*************** HELPERS ****************/

void PRINT_HEX(uint32_t x,std::string msg) 
{
    do { 
        std::cout << msg  << " = 0x" << std::hex << std::setw(8) << std::setfill('0') << x << std::dec << std::endl; 
    } while (0);
}
void PRINT_BIN(uint32_t x, std::string msg) 
{
    do { 
        std::cout << msg << " = 0b" << std::bitset<32>(x) << std::endl; 
    } while (0);
}


// Default Constructor
bloom_filter::bloom_filter()
{
    m_size = 0;
    m_indices = 0;
    m_count = 0;
    m_num_hashes= 0;
}

// Argument Constructor
bloom_filter::bloom_filter(uint32_t size, uint32_t num_hashes)
{
    m_size = size;
    m_indices = size >> 5;
    m_count = 0;
    m_num_hashes= num_hashes;
    
    for(int i = 0; i < m_indices; ++i)
        m_bloom_filter[i] = 0;
}


// Add function
void bloom_filter::add(uint32_t key)
{
    PRINT("\tAdding key: " + STR(key))
    // Given the key, lets generate m_num_hashes;

    // if we are only using 1 hash function
    if(m_num_hashes == 1)
    {
        // hash using murmur3
        uint32_t hash_val;
        MurmurHash3_x86_32(&key, 0, &hash_val);
        #ifdef DEBUG
        PRINT("\t\tHASHED:")
        PRINT_HEX(hash_val, "\t\t\t");
        PRINT_BIN(hash_val, "\t\t\t");

        PRINT("\t\tINDEX:");
        PRINT_HEX((hash_val >> 5) % m_indices, "\t\t\t");
        PRINT_BIN((hash_val >> 5) % m_indices, "\t\t\t");

        PRINT("\t\tOFFSET:")
        PRINT_HEX((hash_val & 0b11111), "\t\t\tval= ");
        PRINT_BIN((1 << (hash_val & 0b11111)), "\t\t\t");
        #endif

        // first five bits are the bit-offset
        // modulo it by the size of the bloom filter, and then set the correct bit in the array
        m_bloom_filter[(hash_val >> 5) % m_indices] |= (1 << (hash_val & 0b11111));
        
    }
    // ONLY ACCOMODATE FOR 1 HASH FUNCTION
    // else
    // {
    //     std::cout << "################################################\n";
    //     std::cout << "################################################\n";
    //     std::cout << "##                   ERROR                    ##\n";
    //     std::cout << "################################################\n";
    //     std::cout << "################################################\n";
    //     exit();

    // }
    m_count++;
}

bool bloom_filter::contains(uint32_t key)
{
    // Given the key, lets generate m_num_hashes;

    PRINT("\tChecking key: " + STR(key))

    // if we are only using 1 hash function
    if(m_num_hashes == 1)
    {
        // hash using murmur3
        uint32_t hash_val;
        MurmurHash3_x86_32(&key, 0, &hash_val);
        #ifdef DEBUG
        PRINT("\t\tHASHED:")
        PRINT_HEX(hash_val, "\t\t\t");
        PRINT_BIN(hash_val, "\t\t\t");

        PRINT("\t\tINDEX:");
        PRINT_HEX((hash_val >> 5) % m_indices, "\t\t\t");
        PRINT_BIN((hash_val >> 5) % m_indices, "\t\t\t");

        PRINT("\t\tOFFSET:")
        PRINT_HEX((hash_val & 0b11111), "\t\t\tval");
        PRINT_BIN((1 << (hash_val & 0b11111)), "\t\t\t");
        #endif
        // first five bits are the bit-offset
        // modulo it by the size of the bloom filter, and then see if correct bit is set
        if(m_bloom_filter[(hash_val >> 5) % m_indices] & (1 << (hash_val & 0b11111)))
            return true;
        else
            return false;


    }
    // ONLY ACCOMODATE FOR 1 HASH FUNCTION
    // else
    // {
    //     std::cout << "################################################\n";
    //     std::cout << "################################################\n";
    //     std::cout << "##                   ERROR                    ##\n";
    //     std::cout << "################################################\n";
    //     std::cout << "################################################\n";
    //     exit();

    // }
}

/********* Getters **********/
uint32_t bloom_filter::get_size()
{
    return m_size;
}

uint32_t bloom_filter::get_count()
{
    return m_count;
}

uint32_t bloom_filter::get_num_hashes()
{
    return m_num_hashes;
}


 