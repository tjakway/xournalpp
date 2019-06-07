#include "map-to-output/MappedRectangle.h"
#include "util/Rectangle.h"

#include <gtk/gtk.h>

namespace {
    Rectangle getWidgetAbsoluteDimensions(GtkWidget* widget)
    {
        GtkAllocation allocation;
    }
}

MappedRectangle::MappedRectangle(
        double aspectRatio,
        unsigned int offsetFromTop,
        cairo_t*, const Rectangle& winAbs)
{

}

Rectangle* MappedRectangle::getMapToOutputRect() const
{
    return mapToOutputRect.get();
}

Rectangle* MappedRectangle::getCairoOutlineRect() const
{
    return cairoOutlineRect.get();
}
