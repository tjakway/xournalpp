#include "map-to-output/XSetWacomDraw.h"

#include "map-to-output/MapToOutputUtil.h"

void XSetWacomDraw::drawOutline(double lineWidth,
        int color,
        cairo_t* cairo, double x, double y, double w, double h)
{
    MapToOutputUtil::checkDimensions(x, y, w, h);

    const char* funcName = __func__;
    const auto drawRect = 
        [cairo](double _x, double _y, double _w, double _h)
        {
            MapToOutputUtil::checkDimensions(_x, _y, _w, _h);
            cairo_rectangle(cairo, _x, _y, _w, _h);
        };

    //left bar
    drawRect(x, y, lineWidth, h);

    //top bar
    drawRect(x, y, w, lineWidth);

    //bottom bar
    drawRect(x, y + h - lineWidth, w, lineWidth);

    //right bar
    drawRect(x + w - lineWidth, y, lineWidth, h);
}

void XSetWacomDraw::drawOutline(const MapToOutputConfig& config,
        cairo_t* cairo, double x, double y, double w, double h)
{
    drawOutline(
            config.mappedAreaOutlineWidth,
            config.mappedAreaOutlineColor,
            cairo, x, y, w, h);
}
