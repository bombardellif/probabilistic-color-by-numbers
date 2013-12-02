#ifndef STATISTICSCOLLECTOR_H
#define STATISTICSCOLLECTOR_H

#include <map>
#include <string>
#include "distribution.h"

class StatisticsCollector
{
private:
    std::vector<AbsDistribution*> distributions;

    int getDir(std::string dir, std::vector<std::string> &files);
public:
    StatisticsCollector();

    void collectData(std::string directory);
};

#endif // STATISTICSCOLLECTOR_H
