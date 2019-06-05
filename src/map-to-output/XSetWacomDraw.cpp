#include "map-to-output/XSetWacomDraw.h"

#include "map-to-output/MapToOutputUtil.h"

void XSetWacomDraw::drawOutline(const MapToOutputConfig& config,
        cairo_t* cairo, int x, int y, int w, int h)
{
    MapToOutputUtil::checkDimensions(x, y, w, h);

}
