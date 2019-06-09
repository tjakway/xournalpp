#pragma once

#include "util/Rectangle.h"
#include "map-to-output/MapToOutputError.h"

#include <memory>

#include <cairo/cairo.h>

class MappedRectangle
{
    std::unique_ptr<Rectangle> mapToOutputRect, cairoOutlineRect;


    static std::unique_ptr<Rectangle> mkMapToOutputRect(
            const Rectangle&, const Offsets&);

    static void checkCairoOutlineRect(const Rectangle&);
    static void checkMapToOutputRect(const Rectangle&);
    static void checkRect(const std::string&, const Rectangle&);

    static void checkOffsets(const Rectangle&, const Offsets&);

    MappedRectangle(const Rectangle&, const Rectangle&);
public:
    struct Offsets
    {
        const unsigned int fromTop,
              fromLeft, fromRight;
    };

    MappedRectangle(
            cairo_t*,
            double aspectRatio,
            const Rectangle& winAbs,
            const Offsets&);

    MappedRectangle(const MappedRectangle&);

    virtual ~MappedRectangle() {}


    Rectangle* getMapToOutputRect() const;
    Rectangle* getCairoOutlineRect() const;

    MappedRectangle move(int amount) const;
};

class MappedRectangleError : public MapToOutputError
{
public:
    MappedRectangleError(const std::string& x)
        : MapToOutputError(x)
    {}

    virtual ~MappedRectangleError() {}
};


class BadOffsetsError : public MappedRectangleError
{
public:
    BadOffsetsError(const std::string& x)
        : MappedRectangleError(x)
    {}

    virtual ~BadOffsetsError() {}
};

/**
 * thrown for Rectangle dimensions errors
 */
class BadRectError : public MappedRectangleError
{
public:
    BadRectError(const std::string& x)
        : MappedRectangleError(x)
    {}

    virtual ~BadRectError() {}
};


