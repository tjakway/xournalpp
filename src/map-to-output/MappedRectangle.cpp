#include "map-to-output/MappedRectangle.h"
#include "util/Rectangle.h"

#include <gtk/gtk.h>

#include <sstream>

namespace {
    std::string printRect(const Rectangle& r)
    {
        std::ostringstream ss;
        ss << "{ x=" <<
            r.x << ", y=" << r.y <<
            ", w=" << r.w << ", h=" r.h << " }";

        return ss.str();
    }

    std::string printOffsets(const MappedRectangle::Offsets& offsets)
    {
        std::ostringstream ss;
        ss << "{ fromTop=" << offsets.fromTop <<
            ", fromLeft=" << offsets.fromLeft <<
            ", fromRight=" << offsets.fromRight <<
            " }";
        return ss.str();
    }
}

MappedRectangle::MappedRectangle(
        const Rectangle& _mapToOutputRect,
        const Rectangle& _cairoOutlineRect)
    mapToOutputRect(new Rectangle(_mapToOutputRect)),
    cairoOutlineRect(new Rectangle(_cairoOutlineRect))
{}

MappedRectangle::MappedRectangle(
    cairo_t* cairo,
    double aspectRatio,
    const Rectangle& winAbs,
    const Offsets& offsets);
{

}

MappedRectangle::MappedRectangle(const MappedRectangle& other)
    : MappedRectangle(*other.mapToOutputRect, *other.cairoOutlineRect)
{}

Rectangle* MappedRectangle::getMapToOutputRect() const
{
    return mapToOutputRect.get();
}

Rectangle* MappedRectangle::getCairoOutlineRect() const
{
    return cairoOutlineRect.get();
}

MappedRectangle MappedRectangle::move(int amount) const
{
    if(amount == 0)
    {
        
    }
}

void MappedRectangle::checkRect(const std::string& name, const Rectangle& r)
{
    if(r.x < 0 || r.y < 0 || r.w <= 0 || r.h <= 0)
    {
        std::ostringstream ss;
        ss << "Expected Rectangle " << name << 
            " to have x, y >= 0 and w, h > 0" <<
            " but got " << printRect(r);

        throw BadRectError(ss.str());
    }
}

void MappedRectangle::checkCairoOutlineRect(const Rectangle& rect)
{
    return checkRect("cairoOutlineRect", rect);
}

void MappedRectangle::checkMapToOutputRect(const Rectangle& rect)
{
    return checkRect("mapToOutputRect", rect);
}

void MappedRectangle::checkOffsets(const Rectangle& cairoOutlineRect,
        const Offsets& offsets)
{
    
    if((cairoOutlineRect.width - offsets.fromLeft - offsets.fromRight) <= 0)
    {
        std::ostringstream ss;
        ss << "Offsets " << printOffsets(offsets) << 
            " exceed cairoOutlineRect " << printRect(cairoOutlineRect);
        throw BadOffsetsError(ss.str());
    }
}

std::unique_ptr<Rectangle> MappedRectangle::mkMapToOutputRect(
        const Rectangle& cairoOutlineRect, const Offsets& offsets)
{
    checkCairoOutlineRect(cairoOutlineRect);
    checkOffsets(cairoOutlineRect, offsets);

    const double boxWidth = cairoOutlineRect.width 
        - offsets.fromLeft - offsets.fromRight;

    const double boxHeight = (1.0 / aspectRatio) * boxWidth;

    const double x = offsets.fromLeft + cairoOutlineRect.x;
    const double y = offsets.fromTop + cairoOutlineRect.y;

    const Rectangle lowestPossibleRect = Rectangle(x, cairoOutlineRect.height - boxHeight,
            boxWidth, boxHeight);


    const bool offBottom = offsets.fromTop > cairoOutlineRect.height;
    if(offBottom)
    {
        return std::unique_ptr<Rectangle>(new Rectangle(lowestPossibleRect));
    }
    else
    {
        return std::unique_ptr<Rectangle>(new Rectangle(x, y, boxWidth, boxHeight));
    }
}
