#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H

#include <vector>
#include <map>
#include <algorithm>

class Distribution
{
private:
    std::vector<double> data;
    std::map<double, int> frequency;
    bool sorted;
    int count;

public:
    Distribution();

    void add(double x);
    double probability(double x);
};

#endif // DISTRIBUTION_H
