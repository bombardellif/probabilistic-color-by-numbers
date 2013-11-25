#ifndef SEGMENTEDIMAGE_H
#define SEGMENTEDIMAGE_H

#include <QImage>
#include <map>
#include <vector>
#include "segment.h"
#include "colorgroup.h"

class SegmentedImage : public QImage
{
private:
    std::map<int, ColorGroup> colorGroups;

    void labelSegment(uint *pixels, int pos, Segment &seg, bool *notVisited);
public:

    SegmentedImage();
    SegmentedImage(const QString &fileName, const char *format = 0);

    void segment();
};

#endif // SEGMENTEDIMAGE_H
