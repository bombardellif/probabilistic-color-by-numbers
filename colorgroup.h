#ifndef COLORGROUP_H
#define COLORGROUP_H

#include <vector>
#include "segment.h"
#include "distribution.h"

class ColorGroup
{
private:
    std::vector<Segment> segments;
    std::vector<std::pair<char, Numeric> > properties;
    std::vector<Segment*> mainSegments;
    Color color;
    QImage *image;

public:
    ColorGroup();
    ColorGroup(QImage *image);

    void addSegment(Segment &seg);
    void transformColor(QColor color);
    int count();
    int countMain();
    Color getColor();
    void setColor(QColor &color);
    QImage *getImage();
    void separateNoise(float noiseThreshold);
    std::vector<std::pair<char, Numeric> > &getProperties();
    void deepCopyTo(ColorGroup *to);

    double score(std::vector<std::pair<char, AbsDistribution*> > &distribution, int &index);
};

#endif // COLORGROUP_H
