#ifndef ATOMIC_COUNTER_H
#define ATOMIC_COUNTER_H

#include "ICounter.h"
#include <atomic>

class AtomicCounter : public ICounter {
private:
    std::atomic<int64_t> atomic_count;

public:
    AtomicCounter() : atomic_count(0) {}

    void increment() override {
        atomic_count.fetch_add(1, std::memory_order_relaxed);
    }

    int64_t get() const override {
        return atomic_count.load(std::memory_order_relaxed);
    }

    void reset() override {
        atomic_count.store(0, std::memory_order_relaxed);
    }
};

#endif