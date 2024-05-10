#ifndef LOG_PROCESSOR_HPP
#define LOG_PROCESSOR_HPP


#include "common.hpp"
#include "bloom_filter.hpp"

/* 
Log Processor

Represents API to our FPGA

Log ingress == log_processor.ingress()
*/
class log_processor
{
    public:
        log_processor(struct rule r[NUM_RULES], uint32_t num_rules, uint32_t num_blooms);

        struct log_result ingress(struct log log);
        uint32_t get_logs_processed();

    private:
        bloom_filter m_bloom_filters[NUM_BLOOM_FILTERS];
        struct rule m_rules[NUM_RULES];
        
        uint32_t m_logs_processed;
        uint32_t m_num_rules;
        uint32_t m_num_blooms;
};

#endif