#include "map-to-output/WithXSetWacomController.h"
#include "map-to-output/MapToOutputConfig.h"

#include <memory>
#include <unordered_set>

static const MapToOutputConfig::DeviceRegexes myDeviceRegexes {
    std::unordered_set<std::string> {
        "Wacom Intuos S 2 Pad pad",
        "Wacom Intuos S 2 Pen stylus"
    },
    "Wacom Intuos S 2 Pen stylus"
};
    

WithXSetWacomController::WithXSetWacomController(bool enabled)
    //TODO: read settings
    : config(myDeviceRegexes)
{
    if(enabled)
    {
        controller = 
            std::unique_ptr<XSetWacomController>(new XSetWacomController)
    }
}


