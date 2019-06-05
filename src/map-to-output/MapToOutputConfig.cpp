#include "MapToOutputConfig.h"

const std::string MapToOutputConfig::Defaults::xsetwacomExecutable = 
    "xsetwacom";

const bool MapToOutputConfig::Defaults::restoreDesktopMappingOnError =
    false;

const bool MapToOutputConfig::Defaults::drawMappedAreaOutline = true;

const unsigned int MapToOutputConfig::Defaults::mappedAreaOutlineWidth = 1;


MapToOutputConfig::MapToOutputConfig(
    const std::string& _xsetWacomExecutable,
    const std::vector<std::string>& _deviceRegexes,
    bool _restoreDesktopMappingOnError,
    bool _drawMappedAreaOutline,
    unsigned int _mappedAreaOutlineWidth)
    : xsetWacomExecutable(_xsetWacomExecutable),
    deviceRegexes(_deviceRegexes),
    restoreDesktopMappingOnError(_restoreDesktopMappingOnError),
    drawMappedAreaOutline(_drawMappedAreaOutline),
    mappedAreaOutlineWidth(_mappedAreaOutlineWidth)
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
