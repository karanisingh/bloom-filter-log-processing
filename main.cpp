//main.cpp

#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <iostream>
#include <thread>
#include <chrono>


#include "src/cpu/common.hpp"
#include "src/cpu/log_processor.hpp"






double calculate_false_positive(double k, double n, double m) {
    double exponent = -k * n / m;
    double base = 1 - exp(exponent);
    double result = pow(base, k);
    return result;
}


int main(int argc, char *argv[])
{
    std::vector<std::string> violations;

    //
    //  PROCESS FIRST LINE
    //  <ancestor>,<descendant>
    //

    std::ifstream input(argv[1]);
    std::string line;
    std::getline(input, line);

    // Statically define our rules
    

    std::stringstream linestream(line);
    std::string cell;

    std::getline(linestream, cell, ',');
    int an = std::stoi(cell);
    std::getline(linestream, cell);
    int de = std::stoi(cell);

    struct rule rules[NUM_RULES];
    rules[0] = rule(an, de);

    //
    // Create log-processor with given rule
    //
    log_processor lp(rules, NUM_RULES, NUM_BLOOM_FILTERS);
    struct log_result r;
    struct log I;
    int p, c, t;


    auto start = std::chrono::high_resolution_clock::now();
    
    while (std::getline(input, line))
    {
        //std::this_thread::sleep_for(std::chrono::milliseconds(10));
        PRINT("\nProcessing log: [" + line + "]")

        std::stringstream linestream(line);
        std::string cell;

        std::getline(linestream, cell, ',');
        p = std::stoi(cell);

        std::getline(linestream, cell, ',');
        c = std::stoi(cell);

        std::getline(linestream, cell);
        t = std::stoi(cell);

        // send to be process
        I.parent_pid=p;
        I.child_pid=c;
        I.task_id=t;
        r = lp.ingress(I);


        //if bloom_size >0, we have a result
        if(r.bloom_size)
        {
            std::string x = "\tRULE VIOLATION (" + STR(r.ancestor) + " --> " + STR(r.descendant) + ") found for chain [  ] -->...--> " +STR(p) + " --> " + STR(c) + "(" + STR(t) + ")\t FP Prob = " + STR(calculate_false_positive(r.bloom_hashes, r.bloom_count, r.bloom_size));
            std::cout << x << std::endl;
            violations.push_back(x);

        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // Output the elapsed time
    std::cout << "Time taken by CPU: process_log(): " << duration.count() << " milliseconds" << std::endl;

    std::cout << "Final Violations:" << std::endl;
    for(auto x: violations)
        std::cout << '\t' << x << std::endl;

    return 0;
}
