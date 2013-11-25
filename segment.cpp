#include "segment.h"

Segment::Segment()
{
}

Segment::Segment(QColor color)
{
    this->color = color;
}

Color& Segment::getColor()
{
    return this->color;
}

void Segment::addPixel(int pixelPos)
{
    this->pixels.push_back(pixelPos);
}
