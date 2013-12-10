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

ColorGroup::ColorGroup(QImage *image, QColor color)
    :image(image)
{
    this->setColor(color);
}

void ColorGroup::setTotalSegments(int num) {
    this->totalSegments=num;
}

float ColorGroup::relativeNumberofSegments() {
    return (float) this->count()/this->totalSegments;
}

// Segment size Statistics
void ColorGroup::SegmentStatistics() {
    int num_elements=0;
    int tot=0;
    int minimo=INF;
    int iter,i;
    int maximum=0;
    for (i=0; i < this->count(); i++) {
        iter=this->segments[i].count();
        tot+=iter;
        num_elements++;
        minimo = std::min(iter , minimo);
        maximum = std::max(iter,maximum);
    }
    this->mean_segs=(float)tot/num_elements;
    this->max_num_elements=maximum;
    this->min_num_elements=minimo;
}

float ColorGroup::getRelativeSize() {
    int group_size=0;
    int total_size;
    for (int i=0; i < this->count(); i++) {
        group_size+=this->segments[i].count();
    }
    total_size=this->image->height()*this->image->width();
    return (float) group_size/total_size;
}

int ColorGroup::getLightness()
{
    return this->color.lightness();
}

int ColorGroup::getSaturation()
{
    return this->color.saturation();
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
    this->properties.push_back(std::pair<char, Numeric>());
    this->properties[0].first = 'd';
    this->properties[0].second.d = (double)this->getRelativeSize();

    this->SegmentStatistics();

    this->properties.push_back(std::pair<char, Numeric>());
    this->properties[1].first = 'd';
    this->properties[1].second.d = (double)this->mean_segs;

    this->properties.push_back(std::pair<char, Numeric>());
    this->properties[2].first = 'i';
    this->properties[2].second.i = this->max_num_elements;

    this->properties.push_back(std::pair<char, Numeric>());
    this->properties[3].first = 'i';
    this->properties[3].second.i = this->min_num_elements;

    this->properties.push_back(std::pair<char, Numeric>());
    this->properties[4].first = 'i';
    this->properties[4].second.i = this->getLightness();

    this->properties.push_back(std::pair<char, Numeric>());
    this->properties[5].first = 'i';
    this->properties[5].second.i = this->getSaturation();

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
        int j, n;

        double value, segmentSum=0,
                weight = 1.0f / 7;//this->properties.size();

        // Calcula propriedades do color group (TODO)
        double ownScore=1;
        for (j=0; j<6; j++) {
            if (this->properties[j].first == 'i') {

                value = (double)this->properties[j].second.i;
                ownScore *= weight * ((Distribution*)distribution[index].second)->probability(value);

            } else if (this->properties[j].first == 'd') {

                value = this->properties[j].second.d;
                ownScore *= weight * ((Distribution*)distribution[index].second)->probability(value);
            }
            index++;
        }

        // Itera nos segmentos
        for (j=0, n=this->mainSegments.size(); j < n; j++) {

            // Obtem scores do segmento
            segmentSum += weight * this->mainSegments[j]->score(distribution, index);
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
