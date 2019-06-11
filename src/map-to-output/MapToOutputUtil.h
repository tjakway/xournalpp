#pragma once

#include "util/Rectangle.h"

#include <utility>

#include <cairo/cairo.h>
#include <gtk/gtk.h>

class MapToOutputUtil
{
    MapToOutputUtil() = delete;
public:
    static void checkDimensions(bool, int, int, int, int);

    static void getCairoProjection(cairo_t*);

    static GdkRectangle widgetGetRectInScreen(GtkWidget *widget);
    static void widgetGetTranslateCoordinates(GtkWidget*);
    static void printWindowOrigin(GtkWidget*);

    static std::pair<int, int> getAbsoluteWidgetPosition(GtkWidget*);
    static Rectangle getAbsoluteWidgetRect(GtkWidget*);

    static bool stringIsNonWhitespace(const std::string&);
};

