#ifndef COMMON_HPP
#define COMMON_HPP


#include <iostream>
#include <string>
#include <cstdint>
#include <cstdlib>
#include <bitset>
#include <iomanip>


/********************** Defines **********************/

// Defines the number of rules
#define NUM_RULES   1

// Defines the number of bits dedicated to a bloom filter containter
// :"arbitrarily" pick 2^18 bits
#define BLOOM_FILTER_SIZE   262144

// Defines the number of bloom filters we want to accomodate for. Would
#define NUM_BLOOM_FILTERS   1

// Defines the number of hash functions to be used per BLoom filter
#define NUM_HASHES  1

// Allows for debug statements
//#define DEBUG

/********************** Helper Macros **********************/

#ifdef DEBUG
#define PRINT(x) std::cout << x << std::endl;
#else
#define PRINT(x)  //
#endif

#define STR(x) std::to_string(x)
// void PRINT_HEX(uint32_t x,std::string msg) 
// {
//     do { 
//         std::cout << msg  << " = 0x" << std::hex << std::setw(8) << std::setfill('0') << x << std::dec << std::endl; 
//     } while (0);
// }
// void PRINT_BIN(uint32_t x, std::string msg) 
// {
//     do { 
//         std::cout << msg << " = 0b" << std::bitset<32>(x) << std::endl; 
//     } while (0);
// }
/********************** Custom Datastructures **********************/


/* 
log struct

logs will be of form "PID X creates PID Y: TASK"
*/
struct log{
    uint32_t parent_pid;
    uint32_t child_pid;
    uint32_t task_id;

    // constructor
    log() {}
    log(uint32_t p, uint32_t c, uint32_t t): parent_pid(p), child_pid(c), task_id(t) {}
};


/*
Rule struct

Houses rules that we want to lookout for

*/
struct rule{
    uint32_t ancestor;
    uint32_t descendant;

    rule() : ancestor(0), descendant(0) {}
    rule(uint32_t r, uint32_t l): ancestor(r), descendant(l) {}
};


/*
mlog result struct

Houses information needed to signal if a rule has been violated, as well as information to calculate false positive probability
*/
struct log_result{
    uint32_t bloom_size;
    uint32_t bloom_count;
    uint32_t bloom_hashes;
    uint32_t ancestor;
    uint32_t descendant;

    log_result() : bloom_size(0), bloom_count(0), bloom_hashes(0), ancestor(0), descendant(0) {}
    log_result(uint32_t m, uint32_t n, uint32_t k, uint32_t a, uint32_t d): bloom_size(m), bloom_count(n), bloom_hashes(k), ancestor(a), descendant(d) {}
};



#endif