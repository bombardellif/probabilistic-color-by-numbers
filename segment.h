#ifndef SEGMENT_H
#define SEGMENT_H

#include <vector>
#include <string>
#include <QImage>
#include "color.h"

union Numeric
{
    int i;
    double d;
};

class Segment
{
private:
    QImage *image;
    std::vector<int> pixels;
    Color color;
    std::vector<std::pair<char, Numeric> >properties;

    double relativeSize();
public:
    Segment();
    Segment(QImage *image);
    Segment(QColor color, QImage *image);

    Color& getColor();
    void setColor(QColor color, bool colorPixels=false);
    void addPixel(int pixelPos);
    int count();
    std::vector<std::pair<char, Numeric> > &getProperties();
};

#endif // SEGMENT_H
