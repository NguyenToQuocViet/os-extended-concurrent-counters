#ifndef CAS_COUNTER_H
#define CAS_COUNTER_H

#include "ICounter.h"
#include <atomic>
#include <thread> 
#include <chrono> 

class CASCounter : public ICounter {
private:
    std::atomic<int64_t> atomic_count;
    
    const int INITIAL_BACKOFF_NS = 10;
    const int MAX_BACKOFF_NS = 10000;

public:
    CASCounter() : atomic_count(0) {}

    void increment() override {
        int64_t current = atomic_count.load(std::memory_order_relaxed);
        int64_t desired;
        
        int backoff = INITIAL_BACKOFF_NS;

        while (true) {
            desired = current + 1;

            if (atomic_count.compare_exchange_weak(current, desired, 
                                                    std::memory_order_relaxed, 
                                                    std::memory_order_relaxed)) {
                break;
            } else {
                std::this_thread::sleep_for(std::chrono::nanoseconds(backoff));

                if (backoff < MAX_BACKOFF_NS) {
                    backoff *= 2;
                }
            }
        }
    }

    int64_t get() const override {
        return atomic_count.load(std::memory_order_relaxed);
    }

    void reset() override {
        atomic_count.store(0, std::memory_order_relaxed);
    }
};

#endif