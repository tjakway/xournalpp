#include "map-to-output/MapToOutputConfig.h"

const std::string MapToOutputConfig::Defaults::xsetWacomExecutable = 
    "xsetwacom";

const bool MapToOutputConfig::Defaults::restoreDesktopMappingOnError =
    false;

const bool MapToOutputConfig::Defaults::drawMappedAreaOutline = true;

const unsigned int MapToOutputConfig::Defaults::mappedAreaOutlineWidth = 1;
const int MapToOutputConfig::Defaults::mappedAreaOutlineColor = 0xFF0000;


MapToOutputConfig::MapToOutputConfig(
    const std::string& _xsetWacomExecutable,
    const std::vector<std::string>& _deviceRegexes,
    bool _restoreDesktopMappingOnError,
    bool _drawMappedAreaOutline,
    unsigned int _mappedAreaOutlineWidth,
    int _mappedAreaOutlineColor)
    : xsetWacomExecutable(_xsetWacomExecutable),
    deviceRegexes(_deviceRegexes),
    restoreDesktopMappingOnError(_restoreDesktopMappingOnError),
    drawMappedAreaOutline(_drawMappedAreaOutline),
    mappedAreaOutlineWidth(_mappedAreaOutlineWidth),
    mappedAreaOutlineColor(_mappedAreaOutlineColor)
{}

MapToOutputConfig::MapToOutputConfig(
        const std::vector<std::string>& _deviceRegexes)
    : MapToOutputConfig(
            Defaults::xsetWacomExecutable, 
            _deviceRegexes,
            Defaults::restoreDesktopMappingOnError,
            Defaults::drawMappedAreaOutline,
            Defaults::mappedAreaOutlineWidth)
{}
