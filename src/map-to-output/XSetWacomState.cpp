#include "map-to-output/XSetWacomState.h"

#include <utility>

XSetWacomState::XSetWacomState(
        const MappedDevices& _mappedDevices,
        const MappedRectangle& _mappedRectangle)
    : mappedDevices(new MappedDevices(_mappedDevices)),
    mappedRectangle(new MappedRectangle(_mappedRectangle))
{}


void XSetWacomState::moveMappedRectangle(const MappedRectangle::Offsets& newOffsets)
{
    std::lock_guard<std::mutex> {stateMutex};
    
    std::unique_ptr<MappedRectangle> newMappedRectangle(
            new MappedRectangle(mappedRectangle->move(newOffsets)));

    //make sure we're not replacing a valid rect with an invalid one
    if(!(mappedRectangle->valid() && !newMappedRectangle->valid()))
    {
        mappedRectangle = std::move(newMappedRectangle);
    }
}

MappedRectangle::Offsets XSetWacomState::getMappedRectangleOffsets()
{
    std::lock_guard<std::mutex> {stateMutex};
    return mappedRectangle->getOffsets();
}

MappedDevices* XSetWacomState::getMappedDevices()
{
    std::lock_guard<std::mutex> {stateMutex};
    return mappedDevices.get();
}

Rectangle* XSetWacomState::getMappedArea() 
{
    std::lock_guard<std::mutex> {stateMutex};
    return mappedRectangle->getMapToOutputRect();
}
