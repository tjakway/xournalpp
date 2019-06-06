#pragma once

#include <memory>
#include <vector>
#include <string>

class MapToOutputConfig final
{
public:
    using Ptr = std::shared_ptr<const MapToOutputConfig>;

    const std::string xsetWacomExecutable;
    const std::vector<std::string> deviceRegexes;
    
    const bool restoreDesktopMappingOnError,
               drawMappedAreaOutline;

    const double mappedAreaOutlineWidth;

    const int mappedAreaOutlineColor;

    //master constructor
    MapToOutputConfig(
        const std::string&,
        const std::vector<std::string>&,
        bool,
        bool,
        double,
        int);

    MapToOutputConfig(
        const std::vector<std::string>&);

    class Defaults
    {
    public:
        static const std::string xsetWacomExecutable;
        static const bool restoreDesktopMappingOnError,
                          drawMappedAreaOutline;
        static const double mappedAreaOutlineWidth;
        static const int mappedAreaOutlineColor;
    };
};
