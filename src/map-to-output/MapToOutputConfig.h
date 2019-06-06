#pragma once

#include <memory>
#include <unordered_set>
#include <string>

class MapToOutputConfig final
{
public:
    using Ptr = std::shared_ptr<const MapToOutputConfig>;

    const std::string xsetWacomExecutable;

    struct DeviceRegexes
    {
        const std::unordered_set<std::string> deviceRegexes;
        const std::string mainDevice;
    };
    const DeviceRegexes deviceRegexes;

    
    const bool restoreDesktopMappingOnError,
               drawMappedAreaOutline;

    const double mappedAreaOutlineWidth;

    const int mappedAreaOutlineColor;

    //whether to check dimensions at runtime
    const bool checkDimensions;

    //master constructor
    MapToOutputConfig(
        const std::string&,
        const DeviceRegexes&,
        bool,
        bool,
        double,
        int,
        bool);

    MapToOutputConfig(
        const DeviceRegexes&);

    class Defaults
    {
    public:
        static const std::string xsetWacomExecutable;
        static const bool restoreDesktopMappingOnError,
                          drawMappedAreaOutline;
        static const double mappedAreaOutlineWidth;
        static const int mappedAreaOutlineColor;
        static const bool checkDimensions;
    };
};
