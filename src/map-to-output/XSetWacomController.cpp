#include "map-to-output/XSetWacomController.h"


static std::mutex XSetWacomController::exeMutex;

XSetWacomController::XSetWacomController(MapToOutputConfig::Ptr _configPtr)
    : configPtr(_configPtr)
{}

void XSetWacomController::setMapToOutput(int x, int y, int width, int height)
{
    std::lock_guard<std::mutex> {exeMutex};
    
    //TODO: shell out
}
