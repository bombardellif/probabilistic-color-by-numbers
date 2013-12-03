#include "colorgroup.h"

ColorGroup::ColorGroup()
{
}

void ColorGroup::addSegment(Segment &seg)
{
    this->segments.push_back(seg);
}

int ColorGroup::count()
{
    return this->segments.size();
}

int ColorGroup::countMain()
{
    return this->mainSegments.size();
}

void ColorGroup::separateNoise(float noiseThreshold)
{
    this->mainSegments.clear();
    for (int i=0; i<this->segments.size(); i++) {

        if (this->segments[i].count() >= noiseThreshold) {

            this->mainSegments.push_back(&this->segments[i]);
        }
    }
}

std::vector<std::pair<char, Numeric> > &ColorGroup::getProperties()
{
    std::vector<std::pair<char, Numeric> > segProps;

    this->properties.clear();

    int k, m, i=0;
    // Calcula propriedades do color group (TODO)

    // Itera nos segmentos
    for (int j=0, n=this->mainSegments.size(); j < n; j++) {

        // Obtem propriedades do segmento
        segProps = this->mainSegments[j]->getProperties();

        // Itera nas propriedades do segmento
        for (k=0, m=segProps.size(); k<m; k++) {

            this->properties.push_back(segProps[k]);
            i++;
        }
    }

    return this->properties;
}

void ColorGroup::transformColor(QColor color) {
    for (int i=0; i < this->segments.size(); i++)
        this->segments[i].setColor(color, true);
}
