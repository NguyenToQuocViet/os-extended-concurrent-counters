#ifndef I_COUNTER_H
#define I_COUNTER_H

#include <cstdint>

class ICounter {
protected:
    int64_t count;

public:
    //default constructor
    ICounter() : count(0) {}

    //cac ham ao thuan tuy cho cac lop overide
    virtual void increment() = 0;

    virtual int64_t get() const {
        return count;
    }

    virtual void reset() {
        count = 0;
    }

    //destructor
    virtual ~ICounter() = default;
};

#endif // I_COUNTER_H