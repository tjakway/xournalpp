#include "map-to-output/MapToOutputUtil.h"

#include "map-to-output/MapToOutputError.h"

#include <sstream>

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
