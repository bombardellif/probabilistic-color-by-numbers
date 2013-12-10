#ifndef STATISTICSCOLLECTOR_H
#define STATISTICSCOLLECTOR_H

#include <map>
#include <string>
#include "distribution.h"
#include "segmentedimage.h"

class StatisticsCollector
{
private:
    static int K;

    std::vector<std::pair<char, Distribution*> > distributions;
    Distribution scoresDistribution;

    int getDir(std::string dir, std::vector<std::string> &files);
public:
    StatisticsCollector();

    void collectData(std::string directory);
    std::vector<std::pair<double, SegmentedImage*> > sample(int numOfSamples, SegmentedImage &templateImg);
};

#endif // STATISTICSCOLLECTOR_H
