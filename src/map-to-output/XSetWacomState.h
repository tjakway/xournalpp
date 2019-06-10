#include "map-to-output/MappedDevices.h"
#include "map-to-output/MappedRectangle.h"

#include <mutex>

class XSetWacomState
{
    std::mutex stateMutex;

    std::unique_ptr<MappedDevices> mappedDevices;
    std::unique_ptr<MappedRectangle> mappedRectangle;

public:
    XSetWacomState(const MappedDevices&, const MappedRectangle&);
    XSetWacomState(const XSetWacomState&) = delete;

    void moveMappedRectangle(const MappedRectangle::Offsets&);
    MappedRectangle::Offsets getMappedRectangleOffsets();

    MappedDevices* getMappedDevices();

    Rectangle* getMappedArea();

    void clearMappedArea();

    virtual ~XSetWacomState() {}
};
