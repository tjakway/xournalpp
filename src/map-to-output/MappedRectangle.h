#pragma once

#include "util/Rectangle.h"
#include "map-to-output/MapToOutputError.h"

#include <memory>

#include <cairo/cairo.h>

class MappedRectangle
{
public:
    struct Offsets
    {
        const unsigned int fromTop,
              fromLeft, fromRight;
    };

private:
    std::unique_ptr<Rectangle> mapToOutputRect, cairoOutlineRect;
    const Offsets offsets;


    static std::unique_ptr<Rectangle> mkMapToOutputRect(
            const Rectangle&, const Offsets&);

    static void checkCairoOutlineRect(const Rectangle&);
    static void checkMapToOutputRect(const Rectangle&);
    static void checkRect(const std::string&, const Rectangle&);

    static void checkOffsets(const Rectangle&, const Offsets&);

    MappedRectangle(const Rectangle&, const Rectangle&,
            const Offsets&);
public:

    MappedRectangle(
            cairo_t*,
            double aspectRatio,
            const Rectangle& winAbs,
            const Offsets&);

    MappedRectangle(const MappedRectangle&);
    MappedRectangle(MappedRectangle&&);

    virtual ~MappedRectangle() {}


    Rectangle* getMapToOutputRect() const;
    Rectangle* getCairoOutlineRect() const;

    MappedRectangle move(int upDown, int leftRight) const;

    bool valid() const;
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


