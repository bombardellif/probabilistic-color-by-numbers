#include "colorgroup.h"

#include <cmath>

ColorGroup::ColorGroup()
{
    this->image = NULL;
    this->segments.clear();
}

ColorGroup::ColorGroup(QImage *image)
{
    this->image = image;
    this->segments.clear();
}

ColorGroup::ColorGroup(QColor color)
{
    this->setColor(color);
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

Color ColorGroup::getColor()
{
    return this->color;
}

void ColorGroup::setColor(QColor &color)
{
    this->color = color;
}

QImage *ColorGroup::getImage()
{
    return this->image;
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

    int k, m;
    // Calcula propriedades do color group (TODO)

    // Itera nos segmentos
    for (int j=0, n=this->mainSegments.size(); j < n; j++) {

        // Obtem propriedades do segmento
        segProps = this->mainSegments[j]->getProperties();

        // Itera nas propriedades do segmento
        for (k=0, m=segProps.size(); k<m; k++) {

            this->properties.push_back(segProps[k]);
        }
    }

    return this->properties;
}

/**
 * Dado um ponteiro para o ColorGroup destino, o metodo copia
 * os mainSegements
 * @brief ColorGroup::deepCopyTo
 * @param to    ColorGroup destino da copia
 */
void ColorGroup::deepCopyTo(ColorGroup *to)
{
    Segment *seg = NULL;
    int i, n;

    // Copia para vetor segments
    for (i=0, n=this->mainSegments.size(); i<n; i++) {
        seg = new Segment(to->getImage());

        this->mainSegments[i]->deepCopyTo(seg);

        to->addSegment(*seg);

        delete seg;
    }
    // Copia para vetor main segments
    to->separateNoise(0);

    if (seg != NULL) {
        to->setColor(seg->getColor());
    }
}

double ColorGroup::score(std::vector<std::pair<char, Distribution *> > &distribution, int &index)
{
    if (this->properties.size()) {
        double segmentSum=0,
                weight = 1.0f / this->properties.size();

        // Calcula propriedades do color group (TODO)
        double ownScore=0;

        // Itera nos segmentos
        for (int j=0, n=this->mainSegments.size(); j < n; j++) {

            // Obtem scores do segmento
            segmentSum = weight * this->mainSegments[j]->score(distribution, index);
        }

        return std::exp(ownScore + segmentSum);
    }

    return 0;
}

void ColorGroup::transformColor(QColor color) {
    this->color = color;
    for (int i=0; i < this->segments.size(); i++)
        this->segments[i].setColor(color, true);
}
