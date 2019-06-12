#pragma once

#include <memory>

#include "map-to-output/XSetWacomController.h"
#include "map-to-output/MapToOutputConfig.h"

class WithXSetWacomController
{
    std::unique_ptr<XSetWacomController> controller;
    MapToOutputConfig::Ptr config;

public:
    WithXSetWacomController(bool);

    virtual ~WithXSetWacomController() {}
};
