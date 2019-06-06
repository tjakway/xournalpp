#pragma once

#include <string>
#include <utility>
#include <vector>

#include "map-to-output/MapToOutputError.h"
#include "map-to-output/MapToOutputConfig.h"

class XSetWacomShell
{
private:
    MapToOutputConfig::Ptr config;

    std::string runXSetWacom(const std::vector<std::string>& args);

    std::vector<std::string> buildArgv(const std::vector<std::string>&);
    std::unique_ptr<char*> argvToPointers(const std::vector<std::string>&);

    std::pair<std::string, std::string> getDimensions(const std::string&);

public:
    XSetWacomShell(MapToOutputConfig::Ptr);

    double getMainDeviceAspectRatio(const std::string&);

    virtual ~XSetWacomShell() {}
};

//error classes
class XSetWacomShellError : public MapToOutputError
{
    XSetWacomShellError(const std::string& x)
        : MapToOutputError(x)
    {}
};
