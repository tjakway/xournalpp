#include "map-to-output/MapToOutputUtil.h"

#include "map-to-output/MapToOutputError.h"

#include <sstream>

#include <gtk/gtk.h>

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

std::pair<double, double> MapToOutputUtil::getCairoProjection(cairo_t* cairo)
{
    double x = -1, y = -1;

    cairo_user_to_device(cairo, &x, &y);

    return std::make_pair(x, y);
}

//from https://mail.gnome.org/archives/gtk-app-devel-list/2004-November/msg00028.html
GdkRectangle MapToOutputUtil::widgetGetRectInScreen(GtkWidget *widget)
{
    GdkRectangle r;
    gint                      x,y,w,h;
    GdkRectangle    extents;
    GdkWindow       *window;

    window = gtk_widget_get_parent_window(widget); /* getting parent window */
    gdk_window_get_root_origin(window, &x,&y); /* parent's left-top screen coordinates */
    gdk_drawable_get_size(window, &w,&h); /* parent's width and height */
    gdk_window_get_frame_extents(window, &extents); /* parent's extents (including decorations) */
    r.x = x + (extents.width-w)/2 + widget->allocation.x; /* calculating x (assuming: left border size == right border size) */
    r.y = y + (extents.height-h)-(extents.width-w)/2 + widget->allocation.y; /* calculating y (assuming: left border size == right border size == bottom border size) */

    r.width = widget->allocation.width;
    r.height = widget->allocation.height;

    return r;
}
