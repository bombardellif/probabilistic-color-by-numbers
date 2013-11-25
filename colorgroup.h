#ifndef COLORGROUP_H
#define COLORGROUP_H

#include <vector>
#include "segment.h"

class ColorGroup
{
private:
    std::vector<Segment> segments;
    int _count;

public:
    ColorGroup();

    void addSegment(Segment &seg);
    int count();
};

#endif // COLORGROUP_H
