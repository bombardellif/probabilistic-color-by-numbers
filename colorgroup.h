#ifndef COLORGROUP_H
#define COLORGROUP_H
#define INF 999999999

#include <vector>
#include "segment.h"
#include "distribution.h"
#include <cstdio>
#include <iostream>
#include <algorithm>

class ColorGroup
{
private:
    std::vector<std::pair<char, Numeric> > properties;
    std::vector<Segment*> mainSegments;
    Color color;
    QImage *image;
    int totalSegments;

public:
    ColorGroup();
    ColorGroup(QImage *image);
    ColorGroup(QColor color);
    ColorGroup(QImage *image, QColor color);

    float mean_segs;
    int min_num_elements;
    int max_num_elements;
    std::vector<Segment> segments;
    // Segment Size Statistics
    void SegmentStatistics();
     void setTotalSegments(int num);
    float getRelativeSize();
    int getLightness();
    int getSaturation();
    float relativeNumberofSegments();
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

    double score(std::vector<std::pair<char, Distribution*> > &distribution, int &index);
};

#endif // COLORGROUP_H
