#ifndef SEGMENT_H
#define SEGMENT_H

#include <vector>
#include <string>
#include <QImage>
#include <cmath>
#include "color.h"
#include "distribution.h"

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

public:
    Segment();
    Segment(QImage *image);
    Segment(QColor color, QImage *image);
    int getPositionX(int pixel);
    int getPositionY(int pixel);
    Color& getColor();
    float getRelativeSize();
	float Centrality();
	float Elongation();
    void setColor(QColor color, bool colorPixels=false);
    void addPixel(int pixelPos);
    int count();
    std::vector<std::pair<char, Numeric> > &getProperties();
    void deepCopyTo(Segment *to);
    void setPixels(std::vector<int> &pixels);

    double score(std::vector<std::pair<char, Distribution*> > &distribution, int &index);
};

#endif // SEGMENT_H
