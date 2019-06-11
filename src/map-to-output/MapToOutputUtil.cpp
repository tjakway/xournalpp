#include "map-to-output/MapToOutputUtil.h"

#include "map-to-output/MapToOutputError.h"

#include <sstream>
#include <iostream>
#include <utility>
#include <tuple>
#include <cassert>

#include <gtk/gtk.h>
#include <gdk/gdk.h>

class BadDimensionsError : public MapToOutputError
{
public:
    BadDimensionsError(const std::string& x)
        : MapToOutputError(x)
    {}
};

void MapToOutputUtil::checkDimensions(
        bool doCheck,
        int x, int y, int width, int height)
{
    if(doCheck)
    {
        std::ostringstream ss;
        ss << "One or more invalid dimensions: "
            << "x=" << x << ", y=" << y
            << ", w=" << width << ", h=" << height;

        if(x < 0 || y < 0 || width <= 0 || height <= 0)
        {
            throw BadDimensionsError(ss.str());
        }
    }
}

void MapToOutputUtil::getCairoProjection(cairo_t* cairo)
{
    double x = -1, y = -1;

    cairo_user_to_device(cairo, &x, &y);

    std::cout << "cairo_user_to_device: (" << x << ", " << y << ")" << std::endl;
}

/**
 * slightly modified from https://mail.gnome.org/archives/gtk-app-devel-list/2004-November/msg00028.html
 */
GdkRectangle MapToOutputUtil::widgetGetRectInScreen(GtkWidget *widget)
{
    GdkRectangle r;
    gint                      x,y,w,h;
    GdkRectangle    extents;
    GdkWindow       *window;


    window = gtk_widget_get_parent_window(widget); /* getting parent window */
    gdk_window_get_root_origin(window, &x,&y); /* parent's left-top screen coordinates */
    w = gdk_window_get_width(window);
    h = gdk_window_get_height(window);
    gdk_window_get_frame_extents(window, &extents); /* parent's extents (including decorations) */

    GtkAllocation widgetAllocation{};
    gtk_widget_get_allocation(widget, &widgetAllocation);
    r.x = x + (extents.width-w)/2 + widgetAllocation.x; /* calculating x (assuming: left border size == right border size) */
    r.y = y + (extents.height-h)-(extents.width-w)/2 + widgetAllocation.y; /* calculating y (assuming: left border size == right border size == bottom border size) */

    r.width = widgetAllocation.width;
    r.height = widgetAllocation.height;

    return r;
}

/**
 * from https://stackoverflow.com/a/2089257/389943
 */
void MapToOutputUtil::widgetGetTranslateCoordinates(GtkWidget* widget)
{
    gint wx = -1, wy = -1;
    gtk_widget_translate_coordinates(widget, gtk_widget_get_toplevel(widget), 0, 0, &wx, &wy);

    std::cout << "gtk_widget_translate_coordinates: (" << 
        wx << ", " << wy << ")" << std::endl;
}
    

void MapToOutputUtil::printWindowOrigin(GtkWidget* widget)
{
    assert(widget != nullptr);
    gint wx = -1, wy = -1;
    std::tie(wx, wy) = getAbsoluteWidgetPosition(widget);

    std::cout << "gdk_window_get_origin: " << "(" << wx << ", " << wy << ")" << std::endl;
}


std::pair<int, int> MapToOutputUtil::getAbsoluteWidgetPosition(GtkWidget* widget)
{
    assert(widget != nullptr);
    gint wx = -1, wy = -1;
    gdk_window_get_origin (gtk_widget_get_window (widget), &wx, &wy);

    return std::make_pair(wx, wy);
}

Rectangle MapToOutputUtil::getAbsoluteWidgetRect(GtkWidget* widget)
{
    assert(widget != nullptr);
    GtkAllocation alloc{};

    gtk_widget_get_allocation(widget, &alloc);

    int x = -1, y = -1;
    std::tie(x, y) = getAbsoluteWidgetPosition(widget);

    assert(x >= 0);
    assert(y >= 0);
    assert(alloc.width > 0);
    assert(alloc.height > 0);
    return Rectangle(x, y, alloc.width, alloc.height);
}

//see https://stackoverflow.com/questions/6444842/efficient-way-to-check-if-stdstring-has-only-spaces
bool MapToOutputUtil::stringIsNonWhitespace(const std::string& str)
{
    return str.find_first_not_of(' ') != std::string::npos;
}
