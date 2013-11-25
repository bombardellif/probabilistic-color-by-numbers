#ifndef COLOR_H
#define COLOR_H

#include <QColor>

class Color : public QColor
{
public:
    using QColor::operator=;

    Color();
};

#endif // COLOR_H
