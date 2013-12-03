#include "segmentedimage.h"

#include <algorithm>
#include <iostream>
#include <map>

SegmentedImage::SegmentedImage()
{
}

SegmentedImage::SegmentedImage(const QString &fileName, const char *format)
    : QImage(fileName, format)
{
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


void SegmentedImage::segment()
{
    uint *pixels = (uint*)this->bits();
    int n = this->byteCount() / 4;
    bool notVisited[n];

    Segment seg;

    memset(&notVisited, true, n);

    //QImage debug(this->width(), this->height(), QImage::Format_RGB32);
    for (int i=0; i<n; i++) {

        if (notVisited[i]) {
            seg = Segment(this);

            this->labelSegment(pixels, i, seg, notVisited);

            if (!this->colorGroups.count(seg.getColor().rgb()))
                this->colorGroups[seg.getColor().rgb()] = ColorGroup();

            this->colorGroups[seg.getColor().rgb()].addSegment(seg);
            /*
            if (seg.getColor().rgb() == 0xff333333){
                this->colorGroups[seg.getColor().rgb()].transformColor(QColor(255, 0, 0));
            }*/
        }
    }
/*
    // Separa os segmentos que representam menos de 0,05% da area da imagem
    //int debug = 0;
    float noiseThreshold = (this->width() * this->height()) * 0.0005;
    std::map<int, ColorGroup>::iterator iter;
    for (iter = this->colorGroups.begin(); iter!=this->colorGroups.end(); iter++) {

        iter->second.separateNoise(noiseThreshold);

        if (iter->second.countMain()) {
            this->mainColorGroups[iter->first] = &iter->second;
            iter->second.transformColor(QColor(std::min(debug,255), 0, 0));
            debug += 20;
        }
    }
*/
    this->save("testeColor.png");
}

std::vector<std::pair<char, Numeric> > SegmentedImage::getProperties()
{
    std::vector<std::pair<char, Numeric> > props, cgProps;
    std::map<int, ColorGroup*>::iterator cgIter;

    int j, n, i=0;

    // Itera nos color groups
    for (cgIter = this->mainColorGroups.begin(); cgIter != this->mainColorGroups.end(); cgIter++) {

        // Obtem propriedades do color group
        cgProps = cgIter->second->getProperties();

        // Itera nas propriedades do color group
        for (j=0, n=cgProps.size(); j<n; j++) {
            props.push_back(cgProps[j]);
            i++;
        }
    }

    return props;
}
