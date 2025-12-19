#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip> 
#include <memory>  

#include "MutexCounter.h"
#include "AtomicCounter.h"
#include "CASCounter.h"

//so lan lap
const int BENCHMARK_ITERATIONS = 1000000; 

void worker_task(ICounter* counter, int iterations) {
    for (int i = 0; i < iterations; ++i) {
        counter->increment();
    }
}

// -----------------
// TEST CORRECTNESS
// -----------------
bool check_correctness(ICounter* counter, std::string name) {
    counter->reset();
    int num_threads = 4;
    int iterations = 100000; 
    
    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(worker_task, counter, iterations);
    }

    //cho tat ca thread lam xong
    for (auto& t : threads) {
        t.join();
    }

    int64_t expected = (int64_t)num_threads * iterations;
    int64_t actual = counter->get();

    if (expected == actual) {
        std::cout << "[PASS] " << name << ": " << actual << "/" << expected << std::endl;
        return true;
    } else {
        std::cout << "[FAIL] " << name << ": " << actual << " (Expected: " << expected << ")" << std::endl;
        return false;
    }
}

// ----------------------
// PERFORMANCE BENCHMARK
// ----------------------
long long run_benchmark(ICounter* counter, int num_threads) {
    counter->reset();
    
    //start
    auto start_time = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(worker_task, counter, BENCHMARK_ITERATIONS);
    }

    //cho tat ca thread lam xong
    for (auto& t : threads) {
        t.join();
    }

    //end
    auto end_time = std::chrono::high_resolution_clock::now();
    
    return std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
}

int main() {
    //initiation
    MutexCounter mutexCounter;
    AtomicCounter atomicCounter;
    CASCounter casCounter;

    std::cout << "======================================\n";
    std::cout << "    PART 1: CORRECTNESS VERIFICATION  \n";
    std::cout << "======================================\n";
    
    check_correctness(&mutexCounter, "Mutex Counter ");
    check_correctness(&atomicCounter, "Atomic Counter");
    check_correctness(&casCounter, "CAS Counter   ");

    std::cout << "\n======================================\n";
    std::cout << "    PART 2: PERFORMANCE BENCHMARK     \n";
    std::cout << "    (Iterations per thread: " << BENCHMARK_ITERATIONS << ")\n";
    std::cout << "======================================\n";

    std::vector<int> thread_counts = {1, 2, 4, 6, 12, 16, 32, 64}; 

    std::cout << std::setw(10) << "Threads" 
              << std::setw(15) << "Mutex (ms)" 
              << std::setw(15) << "Atomic (ms)" 
              << std::setw(15) << "CAS (ms)" << std::endl;
    std::cout << std::string(55, '-') << std::endl;

    for (int n : thread_counts) {
        long long t_mutex = run_benchmark(&mutexCounter, n);
        long long t_atomic = run_benchmark(&atomicCounter, n);
        long long t_cas = run_benchmark(&casCounter, n);

        std::cout << std::setw(10) << n 
                  << std::setw(15) << t_mutex 
                  << std::setw(15) << t_atomic 
                  << std::setw(15) << t_cas << std::endl;
    }

    std::cout << "\nBenchmark finished. Press Enter to exit.";
    std::cin.get();

    return 0;
}