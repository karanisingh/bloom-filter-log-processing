// process_log.cpp

#include "process_log.hpp"
#include "bloom_filter.hpp"
#include "common.hpp"

#define RULE_ANCESTOR 17
#define RULE_DESCENDANT 44

extern "C"
{
    void process_log(hls::stream<struct log> &input_stream, hls::stream<struct log_result> &output_stream)
    {
        // input
        #pragma HLS INTERFACE axis port = input_stream
        // output
        #pragma HLS INTERFACE axis port = output_stream
        // host communication
        #pragma HLS INTERFACE s_axilite port = return

        // create the bloom filter
        bloom_filter bf(BLOOM_FILTER_SIZE, NUM_HASHES);

        // Process incoming log as it arrives in stream
        while (!input_stream.empty())
        {
            // throughput
            #pragma HLS pipeline II = 1

            struct log i = input_stream.read();
            struct log_result r;
            r.bloom_size = 0;

            if (i.child_pid == RULE_ANCESTOR)
                bf.add(i.child_pid);

            if (bf.contains(i.parent_pid))
            {
                bf.add(i.child_pid);

                if (i.task_id == RULE_DESCENDANT)
                {
                    r.bloom_size = BLOOM_FILTER_SIZE;
                    r.bloom_count = bf.get_count;
                    r.bloom_hashes = NUM_HASHES;
                    r.ancestor = RULE_ANCESTOR;
                    r.descendant = RULE_DESCENDANT;
                }
                output_stream.write(result);
            }
        }
    }
}