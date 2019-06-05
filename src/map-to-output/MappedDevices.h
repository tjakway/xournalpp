#pragma once

#include <vector>
#include <string>
#include <unordered_set>

#include "map-to-output/MapToOutputConfig.h"

class MappedDevices
{
public:
    using MappedDeviceSet = std::unordered_set<std::string>;

private:
    const MappedDeviceSet mappedDevices;

public:
    MappedDevices(const std::vector<std::string>&,
                  const std::string&);

    MappedDevices(const MapToOutputConfig&,
                  const std::string&);

    virtual ~MappedDevices() {}

    const MappedDeviceSet getMappedDevices() const;
};
