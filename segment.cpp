#include "segment.h"

#include <cmath>


double Segment::relativeSize()
{
    return 6.9;// DEBUG
}

Segment::Segment()
{
    this->pixels.clear();
}

Segment::Segment(QImage *image)
{
    this->image = image;
    this->pixels.clear();
}

Segment::Segment(QColor color, QImage *image)
{
    this->color = color;
    this->image = image;
}

Color& Segment::getColor()
{
    return this->color;
}

int Segment::getPositionX(int pixel) {
    int w;
    w=this->image->width();
    return (pixel % w);
}

int Segment::getPositionY(int pixel) {
    return std::floor(pixel/this->image->height());
}

float Segment::getRelativeSize() {
    int tam_img = this->image->width()*this->image->height();
    return (float) this->count()/tam_img;
}

float Segment::Elongation() {
    //calculate bounding box
    float x,y;
    float left = (float) this->image->width() - 1;
    float down = 0;
    float right = 0;
    float up = (float) this->image->height() - 1;
    for (int i=0; i < this->count(); i++) {
        int pix=this->pixels[i];
        x = (float) getPositionX(pix);
        y = (float) getPositionY(pix);
        left = std::min(left,x);
	right = std::max(right,x);
	up = std::min(up,y);
	down = std::max(down,y);
    }

    return 1-(right-left)/(up-down);
}

float Segment::Centrality() {
    int iter;
    float x,y;
    int half_pixels = this->count()/2;
    float distance=0;
    for (iter=0; iter < half_pixels; iter++) { 
	// pega o pixel que esta na metade do caminho
    }
	int pix=this->pixels[iter];
        y = (float) getPositionY(pix);
	x = (float) getPositionX(pix);
    float center_image_x = (float) this->image->width()/2;
    float center_image_y = (float) this->image->height()/2;
    distance=std::sqrt((x-center_image_x)*(x-center_image_x)+(y-center_image_y)*(y-center_image_y)); // distancia para o centro da imagem
    return distance;
}

void Segment::setColor(QColor color, bool colorPixels)
{
    this->color = color;
    if (colorPixels) {
        for (int i=0 ; i < pixels.size(); i++) {
            ((uint*)this->image->bits())[this->pixels[i]] = color.rgb();
        }
    }
}

void Segment::addPixel(int pixelPos)
{
    this->pixels.push_back(pixelPos);
}

int Segment::count()
{
    return this->pixels.size();
}

std::vector<std::pair<char, Numeric> > &Segment::getProperties()
{
    // Calcula as propriedades do segmento
    this->properties.clear();

    this->properties.push_back(std::pair<char, Numeric>());
    this->properties[0].first = 'd';
    this->properties[0].second.d = this->relativeSize();

    return this->properties;
}

void Segment::deepCopyTo(Segment *to)
{
    to->setPixels(this->pixels);
    to->setColor(QColor(to->pixels[0]));
}

void Segment::setPixels(std::vector<int> &pixels)
{
    this->pixels = pixels;
}

double Segment::score(std::vector<std::pair<char, AbsDistribution *> > &distribution, int &index)
{
    double prod=1, value;

    for (int i=0, n=this->properties.size(); i<n; i++) {
        if (this->properties[i].first == 'i') {

            value = this->properties[i].second.i;
            prod *= ((Distribution<int>*)distribution[index].second)->probability(value);

        } else if (this->properties[i].first == 'd') {

            value = this->properties[i].second.d;
            prod *= ((Distribution<double>*)distribution[index].second)->probability(value);
        }
        index++;
    }
    return std::log(prod) * this->count();
}
