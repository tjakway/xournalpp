#include "map-to-output/WithXSetWacomController.h"
#include "map-to-output/MapToOutputConfig.h"

WithXSetWacomController::WithXSetWacomController(bool enabled)
{
    if(enabled)
    {
        //TODO: read settings
        
        controller = 
            std::unique_ptr<XSetWacomController>(new XSetWacomController)
    }
}


