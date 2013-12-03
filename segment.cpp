#include "segment.h"

double Segment::relativeSize()
{
    return 6.9;// DEBUG
}

Segment::Segment()
{
}

Segment::Segment(QImage *image)
{
    this->image = image;
}

Segment::Segment(QColor color, QImage *image)
{
    this->color = color;
    this->image = image;
}

Color& Segment::getColor()
{
    return this->color;
}

void Segment::setColor(QColor color, bool colorPixels)
{
    this->color = color;
    if (colorPixels) {
        for (int i=0 ; i < pixels.size(); i++) {
            ((uint*)this->image->bits())[this->pixels[i]] = color.rgb();
        }
    }
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
