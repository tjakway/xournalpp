#include "map-to-output/MapToOutputConfig.h"

const std::string MapToOutputConfig::Defaults::xsetWacomExecutable = 
    "xsetwacom";

const bool MapToOutputConfig::Defaults::restoreDesktopMappingOnError =
    false;

const bool MapToOutputConfig::Defaults::drawMappedAreaOutline = true;

const double MapToOutputConfig::Defaults::mappedAreaOutlineWidth = 1;
const int MapToOutputConfig::Defaults::mappedAreaOutlineColor = 0xFF0000;

const bool MapToOutputConfig::Defaults::checkDimensions = false;


MapToOutputConfig::MapToOutputConfig(
    const std::string& _xsetWacomExecutable,
    const std::vector<std::string>& _deviceRegexes,
    bool _restoreDesktopMappingOnError,
    bool _drawMappedAreaOutline,
    double _mappedAreaOutlineWidth,
    int _mappedAreaOutlineColor,
    bool _checkDimensions)
    : xsetWacomExecutable(_xsetWacomExecutable),
    deviceRegexes(_deviceRegexes),
    restoreDesktopMappingOnError(_restoreDesktopMappingOnError),
    drawMappedAreaOutline(_drawMappedAreaOutline),
    mappedAreaOutlineWidth(_mappedAreaOutlineWidth),
    mappedAreaOutlineColor(_mappedAreaOutlineColor),
    checkDimensions(_checkDimensions)
{}

MapToOutputConfig::MapToOutputConfig(
        const std::vector<std::string>& _deviceRegexes)
    : MapToOutputConfig(
            Defaults::xsetWacomExecutable, 
            _deviceRegexes,
            Defaults::restoreDesktopMappingOnError,
            Defaults::drawMappedAreaOutline,
            Defaults::mappedAreaOutlineWidth,
            Defaults::mappedAreaOutlineColor,
            Defaults::checkDimensions)
{}
