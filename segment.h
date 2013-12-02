#ifndef SEGMENT_H
#define SEGMENT_H

#include <vector>
#include <string>
#include "color.h"

union Numeric
{
    int i;
    double d;
};

class Segment
{
private:
    std::vector<int> pixels;
    Color color;
    std::vector<std::pair<char, Numeric> >properties;

    double relativeSize();
public:
    Segment();
    Segment(QColor color);

    Color& getColor();
    void setColor(QColor color);
    void addPixel(int pixelPos);
    int count();
    std::vector<std::pair<char, Numeric> > &getProperties();
};

#endif // SEGMENT_H
