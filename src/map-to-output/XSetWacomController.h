#pragma once

#include <mutex>

#include "map-to-output/MapToOutputConfig.h"
#include "map-to-output/MappedDevices.h"
#include "map-to-output/XSetWacomShell.h"

#include <gtk/gtk.h>

class XSetWacomController
{
    const MapToOutputConfig::Ptr configPtr;
    XSetWacomShell shell;

    const MappedDevices::MappedDeviceSet mappedDeviceSet;
    const double mainDeviceAspectRatio;

public:
    XSetWacomController(MapToOutputConfig::Ptr);

    virtual ~XSetWacomController() {}

    void setMapToOutput(int x, int y, int width, int height);
    void setMapToOutput(GtkWidget*);

    void onWindowChanged(int x, int y, int width, int height);
    void onWindowChanged(GtkWidget*);
};
