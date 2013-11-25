#include "colorgroup.h"

ColorGroup::ColorGroup()
{
    this->_count = 0;
}

void ColorGroup::addSegment(Segment &seg)
{
    this->segments.push_back(seg);
    this->_count++;
}

int ColorGroup::count()
{
    return this->_count;
}
