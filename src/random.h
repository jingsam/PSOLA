#ifndef RANDOM_H
#define RANDOM_H


typedef unsigned long ulong;

class Random
{
public:
    Random() {
        this->seed = 0;
    }

    ~Random() {}

    explicit Random(ulong seed) {
        this->seed = seed;
    }


    void srand(ulong seed) {
        this->seed = seed;
    }

    void srand(int i) {
        this->seed += i;
    }

    ulong rand(){
        seed = (A * seed + C) % M;
        return seed;
    }

    double nextDouble() {
        return (double) this->rand() / M;
    }

    // [0, max)
    ulong nextInt(ulong max) {
        return this->rand() % max;
    }

    // [0, p.size())
    ulong nextInt(std::vector<double>& p) {
        double r = this->nextDouble();
        double sum = 0.0;
        for (int i = 0; i < p.size(); ++i) {
            sum += p.at(i);
            if (sum >= r && p.at(i) != 0.0) return i;
        }

        return 0;
    }

private:
    ulong seed;

    static const ulong M = 2147483648UL;
    static const ulong A = 1103515245UL;
    static const ulong C = 12345UL;
};


#endif
