#pragma once

#include <vector>
#include <string>
#include <unordered_set>

#include "map-to-output/MapToOutputConfig.h"

class MappedDevices
{
public:
    struct MappedDeviceSet
    {
        const std::unordered_set<std::string> mappedDevices;
        const std::string mainDevice;
    };

private:
    const MappedDeviceSet mappedDevices;

public:
    MappedDevices(const MapToOutputConfig::DeviceRegexes&,
                  const std::string&);

    MappedDevices(const MapToOutputConfig&,
                  const std::string&);

    virtual ~MappedDevices() {}

    const MappedDeviceSet getMappedDevices() const;
};


//mapped device related errors
class MainDeviceError : public MapToOutputError
{
public:
    MainDeviceError(const std::string& x)
        : MapToOutputError(x)
    {}
};

class TooManyMainDevicesError : public MapToOutputError
{
public:
    TooManyMainDevicesError(const std::string& x)
        : MainDeviceError(x)
    {}
};

class NoMainDeviceError : public MapToOutputError
{
public:
    NoMainDeviceError(const std::string& x)
        : MainDeviceError(x)
    {}
};
