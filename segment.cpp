#include "segment.h"

double Segment::relativeSize()
{
    return 6.9;// DEBUG
}

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

void Segment::setColor(QColor color)
{
    this->color = color;
}

void Segment::addPixel(int pixelPos)
{
    this->pixels.push_back(pixelPos);
}

int Segment::count()
{
    return this->pixels.size();
}

std::vector<std::pair<char, Numeric> > &Segment::getProperties()
{
    // Calcula as propriedades do segmento
    this->properties.clear();

    this->properties.push_back(std::pair<char, Numeric>());
    this->properties[0].first = 'd';
    this->properties[0].second.d = this->relativeSize();

    return this->properties;
}
