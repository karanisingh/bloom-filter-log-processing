#ifndef PROCESS_LOG_H
#define PROCESS_LOG_H

// Includes
#include "common.hpp"
#include <iostream>
#include <hls_stream.h>
#include <ap_int.h>
#include <stdlib.h>


extern "C" {

    void process_log(hls::stream<struct log> &input_stream, hls::stream<struct log_result> &output_stream);
}

#endif