#ifndef MUTEX_COUNTER_H
#define MUTEX_COUNTER_H

#include "ICounter.h"
#include <mutex>

class MutexCounter : public ICounter {
private:
    std::mutex mtx; //o khoa

public:
    //overide cac ham thuan ao cua ICounter
    void increment() override {
        std::lock_guard<std::mutex> lock(mtx);  //lock
        count++;
    }
    
    int64_t get() const override {
        return count;
    }
};

#endif