#include "segmentedimage.h"

#include <iostream>
#include <map>
#include <cstdlib>
#include <ctime>
#include <climits>

SegmentedImage::SegmentedImage()
{
}

SegmentedImage::SegmentedImage(const QString &fileName, const char *format)
    : QImage(fileName, format)
{
}

SegmentedImage::SegmentedImage(uchar *data, int width, int height, QImage::Format format, QImageCleanupFunction cleanupFunction, void *cleanupInfo):
    QImage(data, width, height, format, cleanupFunction, cleanupInfo)
{
}

bool SegmentedImage::operator<(const SegmentedImage &r) const
{
    return bits()[0] < r.bits()[0];
}

void SegmentedImage::labelSegment(uint *pixels, int pos, Segment &seg, bool *notVisited)
{
    short i, n=0;
    int pixVal, maxFreq, modeVal,
        width = this->width(),
        height = this->height();
    std::map<int, short> freq;
    std::map<int, short>::iterator freqIter;
    // INICIO: vetor das posiçoes vizinhas do pixel que podem ser acessada
    int delta[9];
    bool notFirstRow = pos >= width,
        notFirstColumn = pos % width != 0,
        notLastRow = pos/width != height-1,
        notLastColumn = (pos+1) % width != 0;

    // Coloca o pixel central no delta[0]
    delta[n++] = pos;
    // Se nao esta na primeira linha ...
    if (notFirstRow) {
        delta[n++] = pos-width;
        // Se nao esta na primeira linha e coluna ...
        if (notFirstColumn)
            delta[n++] = pos-width-1;
        // Se nao esta na primeira linha ultima coluna ...
        if (notLastColumn)
            delta[n++] = pos-width+1;
    }
    // Se nao esta na ultima linha ...
    if (notLastRow) {
        delta[n++] = pos+width;
        // Se nao esta na ultima linha primeira coluna ...
        if (notFirstColumn)
            delta[n++] = pos+width-1;
        // Se nao esta na ultima linha ultima coluna ...
        if (notLastColumn)
            delta[n++] = pos+width+1;
    }
    // Se nao esta na primeira coluna ...
    if (notFirstColumn)
        delta[n++] = pos-1;
    // Se nao esta na ultima coluna ...
    if (notLastColumn)
        delta[n++] = pos+1;

    // FIM: vetor das posiçoes vizinhas do pixel que podem ser acessada

    // INICIO: moda da vizinhança 8x8
    // calcula frequencia de cada cor
    for (i=0; i<n; i++) {

        pixVal = pixels[delta[i]];
        if (freq.count(pixVal)) {
            freq[pixVal]++;
        } else {
            freq[pixVal] = 1;
        }
    }
    // valor de maxima frequencia
    maxFreq = 0;
    for (freqIter = freq.begin(); freqIter!=freq.end(); freqIter++) {
        if (freqIter->second > maxFreq) {
            modeVal = freqIter->first;
        }
    }
    // FIM: moda da vizinhança 8x8

    // Se segmento ainda nao tem pixel, deve setar a cor para a moda da vizinhança-8x8
    if (!seg.count()) {
        seg.setColor(QColor(modeVal));
    }

    if (QColor(modeVal) == seg.getColor()) {
        notVisited[pos] = false;
        seg.addPixel(pos);

        // rotula os vizinhos 8x8
        for (i=1; i<n; i++) {
            if (notVisited[delta[i]])
                this->labelSegment(pixels, delta[i], seg, notVisited);
        }
    }
}

void SegmentedImage::SeparateNoise()
{
    float noiseThreshold = (this->width() * this->height()) * 0.001;
    std::map<int, ColorGroup>::iterator iter;
    //int r=0,g=0,b=0;

    for (iter = this->colorGroups.begin(); iter!=this->colorGroups.end(); iter++) {

        iter->second.separateNoise(noiseThreshold);

        if (iter->second.countMain()) {

            this->mainColorGroups[iter->first] = &iter->second;

            /*std::cout << r << '.' << g << '.' << b << std::endl;
            if ((r += 10) > 250) {
                r = 255;
                if ((g += 10)>250) {
                    g = 255;
                    if ((b += 10)>250)
                        b = 255;
                }
            }

            iter->second.transformColor(QColor(r, g, b));*/

        }
    }

    //this->save("testeColor.png");
}

void SegmentedImage::segment()
{
    uint *pixels = (uint*)this->bits();
    int n = this->byteCount() / 4;
    bool notVisited[n];

    Segment seg;

    memset(&notVisited, true, n);
    int num_segments=0;
    //QImage debug(this->width(), this->height(), QImage::Format_RGB32);
    for (int i=0; i<n; i++) {

        if (notVisited[i]) {
            seg = Segment(this);

            this->labelSegment(pixels, i, seg, notVisited);

            /*if (!this->colorGroups.count(seg.getColor().rgb())) {
                this->colorGroups[seg.getColor().rgb()] = ColorGroup(QColor(seg.getColor()));
            }*/

            if (!this->colorGroups.count(seg.getColor().rgb()))
                this->colorGroups[seg.getColor().rgb()] = ColorGroup(this, QColor(seg.getColor()));
            num_segments++;
            this->colorGroups[seg.getColor().rgb()].addSegment(seg);
            /*
            if (seg.getColor().rgb() == 0xff333333){
                this->colorGroups[seg.getColor().rgb()].transformColor(QColor(255, 0, 0));
            }*/
        }
    }
    std::map<int, ColorGroup>::iterator iter;
    for (iter = this->colorGroups.begin(); iter != this->colorGroups.end(); iter++) {
        iter->second.setTotalSegments(num_segments);
        /* std::cout << iter->second.getRelativeSize();
        iter->second.SegmentStatistics();
        std::cout << iter->second.max_num_elements << std::endl;
        std::cout << iter->second.min_num_elements << std::endl;
        std::cout << iter->second.mean_segs << std::endl;
        std::cout << iter->second.relativeNumberofSegments() << std::endl;
        */

    }
    /*std::vector<Segment> iter_seg = ;
    for (int i = 0; i < iter->second.segments.size(); i++) {
        std::cout << iter->second.segments[i].Elongation();
    }*/

    // Separa os segmentos que representam menos de 0,05% da area da imagem
    this->SeparateNoise();

}

std::vector<std::pair<char, Numeric> > SegmentedImage::getProperties()
{
    std::vector<std::pair<char, Numeric> > props, cgProps;
    std::map<int, ColorGroup*>::iterator cgIter;

    int j, n;

    // Itera nos color groups
    for (cgIter = this->mainColorGroups.begin(); cgIter != this->mainColorGroups.end(); cgIter++) {

        // Obtem propriedades do color group
        cgProps = cgIter->second->getProperties();

        // Itera nas propriedades do color group
        for (j=0, n=cgProps.size(); j<n; j++) {
            props.push_back(cgProps[j]);
        }
    }

    return props;
}

std::map<int, ColorGroup*> &SegmentedImage::getMainColorGroups()
{
    return this->mainColorGroups;
}

/**
 * Dada uma imagem origem, copia para este(this) objeto os mainColorGroups
 * da origem, realizando uma deep copy, de maneira que copie tambem os segementos
 * dos color groups.
 *
 * @brief SegmentedImage::deepCopySegmentation
 * @param from  Imagem destino
 */
void SegmentedImage::deepCopySegmentation(SegmentedImage from)
{
    std::map<int, ColorGroup*> &cgFrom = from.getMainColorGroups();
    std::map<int, ColorGroup*>::iterator iterCgFrom;
    ColorGroup *cgTo;
    int color;

    this->colorGroups.clear();
    this->mainColorGroups.clear();
    for (iterCgFrom = cgFrom.begin(); iterCgFrom != cgFrom.end(); iterCgFrom++) {

        cgTo = new ColorGroup(this);

        iterCgFrom->second->deepCopyTo(cgTo);

        color = iterCgFrom->second->getColor().rgb();
        this->colorGroups[color] = *cgTo;
        this->mainColorGroups[color] = &this->colorGroups[color];
    }

}

double SegmentedImage::score(std::vector<std::pair<char, Distribution *> > &distribution)
{
    std::map<int, ColorGroup*>::iterator iter;
    double prod = 1;
    int index = 0;

    for (iter = this->mainColorGroups.begin(); iter!=this->mainColorGroups.end(); iter++) {
        prod *= iter->second->score(distribution, index);
    }

    return prod;
}

void SegmentedImage::randomlyColor()
{
    int randomColor;
    std::srand(std::time(NULL));

    std::map<int, ColorGroup*>::iterator iter;
    for (iter=this->mainColorGroups.begin(); iter!=this->mainColorGroups.end(); iter++) {

        randomColor = (int)(std::rand() % (INT_MAX));
        iter->second->transformColor(QColor(randomColor));
    }
}
