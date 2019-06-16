#pragma once

#include <vector>
#include <string>
#include <unordered_set>

#include "map-to-output/MapToOutputConfig.h"
#include "map-to-output/MapToOutputError.h"

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

    MappedDevices(const MappedDevices&);

    virtual ~MappedDevices() {}

    MappedDeviceSet getMappedDevices() const;
};


//mapped device related errors

class MappedDevicesError : public MapToOutputError
{
public:
    MappedDevicesError(const std::string& x)
        : MapToOutputError(x)
    {}

    virtual ~MappedDevicesError() {}
};


class MainDeviceError : public MappedDevicesError
{
public:
    MainDeviceError(const std::string& x)
        : MappedDevicesError(x)
    {}

    virtual ~MainDeviceError() {}
};

class TooManyMainDevicesError : public MainDeviceError
{
public:
    TooManyMainDevicesError(const std::string& x)
        : MainDeviceError(x)
    {}

    virtual ~TooManyMainDevicesError() {}
};

class NoMainDeviceError : public MainDeviceError
{
public:
    NoMainDeviceError(const std::string& x)
        : MainDeviceError(x)
    {}

    virtual ~NoMainDeviceError() {}
};

class DeviceNameExtractionError : public MappedDevicesError
{
public:
    DeviceNameExtractionError(const std::string& x)
        : MappedDevicesError(x)
    {}

    virtual ~DeviceNameExtractionError() {}
};
