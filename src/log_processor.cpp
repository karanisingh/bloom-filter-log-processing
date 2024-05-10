// log_processor.cpp

#include "log_processor.hpp"

// log processor constructor
log_processor::log_processor(struct rule r[NUM_RULES], uint32_t num_rules, uint32_t num_blooms)
{
    m_logs_processed = 0;
    m_num_rules = num_rules;
    m_num_blooms = num_blooms;

    // Initialize our rules list
    // unroll
    for (int i = 0; i < num_rules; ++i)
    {
        m_rules[i] = r[i];
    }

    // Initialize our bloom filters
    // unroll
    for (int i = 0; i < num_blooms; ++i)
    {
        m_bloom_filters[i] = bloom_filter(BLOOM_FILTER_SIZE, NUM_HASHES);
    }
}




struct log_result log_processor::ingress(struct log log)
{
    uint32_t p = log.parent_pid;
    uint32_t c = log.child_pid;
    uint32_t t = log.task_id;

    struct log_result this_result;

    std::cout << "Processing log: " + STR(p) + " --> " + STR(c) + " (" + STR(t) + ")" << std::endl;

    // Iterate through every bloom_filter
    for (int i = 0; i < m_num_blooms; ++i)
    {
        
        
        PRINT("\tChecking if we need to insert at begin")
        // insert if we are the beginning of a work
        if(t == m_rules[i].ancestor)
        {
            PRINT("\t\tInserting into da blooooom")
            m_bloom_filters[i].add(c);
        }

        PRINT("\tChecking bloom filter " + STR(i))
        // Check if process lineage in this bloom filter
        if (m_bloom_filters[i].contains(p))
        {
            PRINT("\t\tProccess lineage found!")
            // If we have a process lineage, and we are a rule, report back
            if (m_rules[i].descendant == t)
            {
                //std::cout << "\tRule violation found!!!!!!!!!!!" << std::endl;
                this_result = log_result(m_bloom_filters[i].get_size(),
                                                m_bloom_filters[i].get_count(),
                                                m_bloom_filters[i].get_num_hashes(),
                                                m_rules[i].ancestor,
                                                m_rules[i].descendant);
            }

            // Add child to process lineage
            m_bloom_filters[i].add(c);
        }
    }

    m_logs_processed++;

    return this_result;
}



uint32_t log_processor::get_logs_processed()
{
    return m_logs_processed;
}