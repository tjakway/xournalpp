#pragma once

#include <memory>
#include <vector>
#include <string>

class MapToOutputConfig final
{
public:
    using Ptr = std::shared_ptr<const MapToOutputConfig>;

    const std::string xsetwacomExecutable;
    const std::vector<std::string> deviceRegexes;
    
    const bool restoreDesktopMappingOnError,
               drawMappedAreaOutline;

    const unsigned int mappedAreaOutlineWidth;

    //master constructor
    MapToOutputConfig(
        const std::string&,
        const std::vector<std::string>&,
        bool,
        bool,
        unsigned int);

    MapToOutputConfig(
        const std::vector<std::string>&);

    class Defaults
    {
    public:
        static const std::string xsetwacomExecutable;
        static const bool restoreDesktopMappingOnError,
                          drawMappedAreaOutline;
        static const unsigned int mappedAreaOutlineWidth;
    };
};
