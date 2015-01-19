#ifndef RANDOM_H
#define RANDOM_H

#include <vector>
#include <ctime>

class Random
{
public:
    Random() {
        this->seed = 0;
    }

    explicit Random(size_t seed) {
        this->seed = seed;
    }

    ~Random() {}

    void srand(size_t seed) {
        this->seed = seed;
    }

    size_t rand(){
        seed = (A * seed + C) % M;
        return seed;
    }

    double nextDouble() {
        return (double) this->rand() / M;
    }

    // [0, max)
    size_t nextInt(size_t max) {
        return this->rand() % max;
    }

private:
    size_t seed;

    static const size_t M = 2147483647;
    static const size_t A = 1103515245;
    static const size_t C = 12345;
};


#endif
