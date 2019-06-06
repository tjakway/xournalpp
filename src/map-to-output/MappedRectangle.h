#pragma once

#include "util/Rectangle.h"

#include <memory>

#include <cairo/cairo.h>

class MappedRectangle
{
    std::unique_ptr<Rectangle> mapToOutputRect, cairoOutlineRect;

public:
    MappedRectangle(double aspectRatio, unsigned int offsetFromTop,
            cairo_t*, const Rectangle& winAbs);
    virtual ~MappedRectangle() {}


    Rectangle* getMapToOutputRect() const;
    Rectangle* getCairoOutlineRect() const;
};
