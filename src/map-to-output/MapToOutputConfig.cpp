#include "MapToOutputConfig.h"

const std::string MapToOutputConfig::Defaults::xsetwacomExecutable = 
    "xsetwacom";

const bool MapToOutputConfig::Defaults::restoreDesktopMappingOnError =
    false;


MapToOutputConfig::MapToOutputConfig(
    const std::string& _xsetWacomExecutable,
    const std::vector<std::string>& _deviceRegexes,
    bool _restoreDesktopMappingOnError)
    : xsetWacomExecutable(_xsetWacomExecutable),
    deviceRegexes(_deviceRegexes),
    restoreDesktopMappingOnError(_restoreDesktopMappingOnError)
{}

MapToOutputConfig::MapToOutputConfig(
        const std::vector<std::string>& _deviceRegexes)
    : MapToOutputConfig(
            Defaults::xsetWacomExecutable, 
            _deviceRegexes,
            Defaults::restoreDesktopMappingOnError)
{}
