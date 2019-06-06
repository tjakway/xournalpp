#pragma once

#include <mutex>

#include "map-to-output/MapToOutputConfig.h"

class XSetWacomController
{
    const MapToOutputConfig::Ptr configPtr;

    static std::mutex exeMutex;

protected:

public:
    XSetWacomController(MapToOutputConfig::Ptr);

    virtual ~XSetWacomController() {}

    void setMapToOutput(int x, int y, int width, int height);

    void onWindowChanged(int x, int y, int width, int height);
};
