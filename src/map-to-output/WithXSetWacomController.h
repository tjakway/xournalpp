#pragma once

#include <memory>

#include "map-to-output/XSetWacomController.h"

class WithXSetWacomController
{
    std::unique_ptr<XSetWacomController> controller;

public:
    WithXSetWacomController(bool);

    virtual ~WithXSetWacomController() {}
};
