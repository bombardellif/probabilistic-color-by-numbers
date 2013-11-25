#ifndef SEGMENT_H
#define SEGMENT_H

#include <vector>
#include "color.h"

class Segment
{
private:
    std::vector<int> pixels;
    Color color;

public:
    Segment();
    Segment(QColor color);

    Color& getColor();
    void addPixel(int pixelPos);
};

#endif // SEGMENT_H
