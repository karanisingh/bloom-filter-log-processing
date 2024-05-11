// fpga_main.cpp

#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <iostream>
#include <chrono>
#include <xrt/xrt_device.h>
#include <xrt/xrt_kernel.h>
#include <xrt/xrt_bo.h>

#include "common.hpp"


// Simplified bloom filter calculation function for the host
double calculate_false_positive(double k, double n, double m) {
    double exponent = -k * n / m;
    return pow(1 - exp(exponent), k);
}

int main(int argc, char *argv[]) {
    std::vector<std::string> violations;
    std::ifstream input(argv[1]);
    std::string line;

    // Initialize device, load xclbin and get kernel
    auto device = xrt::device(0); // Open device at index 0
    auto uuid = device.load_xclbin("log_kern.xclbin");
    auto krnl = xrt::kernel(device, uuid, "process_log");

    // Read the first line to extract rules
    std::getline(input, line);
    std::stringstream linestream(line);
    std::string cell;
    std::getline(linestream, cell, ',');
    int an = std::stoi(cell);
    std::getline(linestream, cell);
    int de = std::stoi(cell);

    // Prepare kernel buffers
    std::vector<log_entry> log_entries;
    std::vector<log_result> results;

    auto start = std::chrono::high_resolution_clock::now();

    // Read log entries from file
    while (std::getline(input, line)) {
        std::stringstream linestream(line);
        log_entry log;
        std::getline(linestream, cell, ',');
        log.parent_pid = std::stoi(cell);
        std::getline(linestream, cell, ',');
        log.child_pid = std::stoi(cell);
        std::getline(linestream, cell);
        log.task_id = std::stoi(cell);
        log_entries.push_back(log);
    }

    // Prepare FPGA buffers and transfer data
    auto bo_in = xrt::bo(device, log_entries.data(), log_entries.size() * sizeof(log_entry), krnl.group_id(0));
    auto bo_out = xrt::bo(device, results.size() * sizeof(log_result), krnl.group_id(1));
    bo_in.sync(XCL_BO_SYNC_BO_TO_DEVICE);

    // Execute the kernel
    auto run = krnl(bo_in, bo_out, (int)log_entries.size());
    run.wait();

    // Retrieve results from the FPGA
    results.resize(log_entries.size()); // Ensure the results vector is sized correctly
    bo_out.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
    bo_out.read(results.data());

    // Process results
    for (const auto& result : results) {
        if (result.result) {
            std::string violation = "\tRULE VIOLATION (" + std::to_string(result.ancestor) + " --> " + std::to_string(result.descendant) +
                                    ") found for chain [  ] -->...--> " + std::to_string(log.parent_pid) + " --> " + std::to_string(log.child_pid) +
                                    "(" + std::to_string(log.task_id) + ")\t FP Prob = " +
                                    std::to_string(calculate_false_positive(result.bloom_hashes, result.bloom_count, result.bloom_size));
            std::cout << violation << std::endl;
            violations.push_back(violation);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Time taken by CPU and FPGA: process_log(): " << duration.count() << " milliseconds" << std::endl;

    std::cout << "Final Violations:" << std::endl;
    for (auto& x : violations)
        std::cout << '\t' << x << std::endl;

    return 0;
}
