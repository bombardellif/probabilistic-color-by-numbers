#ifndef SEGMENTEDIMAGE_H
#define SEGMENTEDIMAGE_H

#include <QImage>
#include <map>
#include <vector>
#include "segment.h"
#include "colorgroup.h"
#include "distribution.h"

class SegmentedImage : public QImage
{
private:
    std::map<int, ColorGroup> colorGroups;
    std::map<int, ColorGroup*> mainColorGroups;

    void labelSegment(uint *pixels, int pos, Segment &seg, bool *notVisited);
    void SeparateNoise();
public:

    SegmentedImage();
    SegmentedImage(const QString &fileName, const char *format = 0);
    SegmentedImage(uchar *data, int width, int height, Format format, QImageCleanupFunction cleanupFunction = 0, void *cleanupInfo = 0);

    bool operator<(const SegmentedImage &r) const;

    void segment();
    std::vector<std::pair<char, Numeric> > getProperties();

    std::map<int, ColorGroup*>& getMainColorGroups();
    void deepCopySegmentation(SegmentedImage from);
    double score(std::vector<std::pair<char, Distribution*> > &distribution);
    void randomlyColor();
};

#endif // SEGMENTEDIMAGE_H
