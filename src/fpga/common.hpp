#ifndef COMMON_HPP
#define COMMON_HPP



#include <cstdlib>


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

#define RULE_ANCESTOR 17
#define RULE_DESCENDANT 44

/* 
log struct

logs will be of form "PID X creates PID Y: TASK"
*/
struct log{
    uint32_t parent_pid;
    uint32_t child_pid;
    uint32_t task_id;
};


/*
Rule struct

Houses rules that we want to lookout for

*/
struct rule{
    uint32_t ancestor;
    uint32_t descendant;
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
};



#endif