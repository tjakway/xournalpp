#include "map-to-output/WithXSetWacomController.h"
#include "map-to-output/MapToOutputConfig.h"

#include <memory>
#include <unordered_set>

//TODO: read settings
static const MapToOutputConfig::DeviceRegexes myDeviceRegexes {
    std::unordered_set<std::string> {
        "Wacom Intuos S 2 Pad pad",
        "Wacom Intuos S 2 Pen stylus"
    },
    "Wacom Intuos S 2 Pen stylus"
};

MapToOutputConfig::Ptr myConfig = std::make_shared<MapToOutputConfig>(myDeviceRegexes);

WithXSetWacomController::WithXSetWacomController(bool enabled)
    : config(myConfig)
{
    if(enabled)
    {
        controller = 
            std::unique_ptr<XSetWacomController>(new XSetWacomController(config));
    }
    else
    {
        controller = nullptr;
    }
}



void WithXSetWacomController::onWindowChanged(GtkWidget* widget)
{
    if(controller)
    {
        controller->onWindowChanged(widget);
    }
}
