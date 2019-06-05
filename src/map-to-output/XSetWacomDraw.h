#pragma once

#include <cairo/cairo.h>

#include "map-to-output/MapToOutputConfig.h"

class XSetWacomDraw
{
protected:
    void drawOutline(const MapToOutputConfig&,
            cairo_t*, int, int, int, int);
};
