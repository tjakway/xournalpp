#pragma once

#include <utility>
#include <cairo/cairo.h>

class MapToOutputUtil
{
    MapToOutputUtil() = delete;
public:
    static void checkDimensions(bool, int, int, int, int);

    static std::pair<double, double> getCairoProjection(cairo_t*);

    GdkRectangle widgetGetRectInScreen(GtkWidget *widget);

};

