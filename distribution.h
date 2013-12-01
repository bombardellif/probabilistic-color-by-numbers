#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H

#include <vector>

class AbsDistribution { };

template <class T>
class Distribution: public AbsDistribution
{
private:
    std::vector<T> data;

public:
    Distribution();

    void add(T x);
    double probability(T x);
};

#endif // DISTRIBUTION_H
