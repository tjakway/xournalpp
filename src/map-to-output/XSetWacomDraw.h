#pragma once

#include <cairo/cairo.h>

#include "map-to-output/MapToOutputConfig.h"

class XSetWacomDraw
{
protected:
    void drawOutline(const MapToOutputConfig&,
            cairo_t*, double, double, double, double);

    void drawOutline(double, int,
            cairo_t*, double, double, double, double);
};
