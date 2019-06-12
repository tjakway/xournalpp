#include "map-to-output/XSetWacomController.h"
#include "map-to-output/MapToOutputUtil.h"


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
    Rectangle r(x, y, width, height);
    shell.setMapToOutput(mappedDeviceSet.mainDevice, &r);
}

void XSetWacomController::onWindowChanged(int x, int y, int width, int height)
{
    setMapToOutput(x, y, width, height);
}

void XSetWacomController::onWindowChanged(GtkWidget* widget)
{
    Rectangle widgetRect = MapToOutputUtil::getAbsoluteWidgetRect(widget);

    onWindowChanged(
        static_cast<int>(widgetRect.x),
        static_cast<int>(widgetRect.y),
        static_cast<int>(widgetRect.width),
        static_cast<int>(widgetRect.height));
}
