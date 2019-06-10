#pragma once

#include "util/Rectangle.h"
#include "map-to-output/MapToOutputError.h"

#include <memory>

#include <cairo/cairo.h>
#include <gtk/gtk.h>

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
    const double aspectRatio;
    const Offsets offsets;


    static std::unique_ptr<Rectangle> mkMapToOutputRect(
            const Rectangle&, double, const Offsets&);

    static void checkCairoOutlineRect(const Rectangle&);
    static void checkMapToOutputRect(const Rectangle&);
    static void checkRect(const std::string&, const Rectangle&);

    static void checkOffsets(const Rectangle&, const Offsets&);

    //master constructor
    MappedRectangle(const Rectangle&, 
            const Rectangle&,
            double,
            const Offsets&);
public:
    MappedRectangle(
            GtkWidget*,
            double aspectRatio,
            const Offsets&);

    MappedRectangle(
            const Rectangle&,
            double aspectRatio,
            const Offsets&);

    MappedRectangle(const MappedRectangle&);
    MappedRectangle(MappedRectangle&&);

    virtual ~MappedRectangle() {}


    Rectangle* getMapToOutputRect() const;
    Rectangle* getCairoOutlineRect() const;

    MappedRectangle move(const Offsets&) const;

    bool valid() const;

    Offsets getOffsets() const { return offsets; }
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


