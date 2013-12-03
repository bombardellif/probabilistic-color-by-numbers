#ifndef COLORGROUP_H
#define COLORGROUP_H

#include <vector>
#include "segment.h"

class ColorGroup
{
private:
    std::vector<Segment> segments;
    std::vector<std::pair<char, Numeric> > properties;
    std::vector<Segment*> mainSegments;

public:
    ColorGroup();

    void addSegment(Segment &seg);
    void transformColor(QColor color);
    int count();
    int countMain();
    void separateNoise(float noiseThreshold);
    std::vector<std::pair<char, Numeric> > &getProperties();
};

#endif // COLORGROUP_H
