#ifndef STATISTICSCOLLECTOR_H
#define STATISTICSCOLLECTOR_H

#include <map>
#include <string>
#include "distribution.h"

class StatisticsCollector
{
private:
    std::map<std::string, AbsDistribution> distributions;

public:
    StatisticsCollector();

    void collectData(std::string directory);
};

#endif // STATISTICSCOLLECTOR_H
