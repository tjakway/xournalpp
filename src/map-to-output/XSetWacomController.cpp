#include "map-to-output/XSetWacomController.h"


XSetWacomController::XSetWacomController(MapToOutputConfig::Ptr _configPtr)
    : configPtr(_configPtr),
    shell(configPtr->xsetWacomExecutable),
    mappedDeviceSet(MappedDevices(
                configPtr->deviceRegexes,
                shell.getDevices()).getMappedDevices()),
    mainDeviceAspectRatio(shell.getMainDeviceAspectRatio(mappedDeviceSet.mainDevice))
{}

void XSetWacomController::setMapToOutput(int x, int y, int width, int height)
{
    //TODO: shell out
}
