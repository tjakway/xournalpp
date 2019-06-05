#include "map-to-output/MapToOutputUtil.h"

#include <stdexcept>
#include <sstream>

class BadDimensionsError
{
public:
    BadDimensionsError(const std::string& x)
        : runtime_error(x)
    {}
};

void MapToOutputUtil::checkDimensions(int x, int y, int width, int height)
{
    std::ostringstream ss;
    ss << "One or more invalid dimensions: " <<
        << "x=" << x << ", y=" << y
        << ", w=" << width << ", h=" << height;

    if(x < 0 || y < 0 || width <= 0 || height <= 0)
    {
        throw BadDimensionsError(ss.str());
    }
}
