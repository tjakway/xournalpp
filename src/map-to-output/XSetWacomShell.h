#pragma once

#include <string>
#include <utility>
#include <vector>
#include <mutex>

#include "util/Rectangle.h"

#include "map-to-output/MapToOutputError.h"
#include "map-to-output/MapToOutputConfig.h"

class XSetWacomShell
{
    //use a mutex to serialize invocations of xsetwacom
    static std::mutex exeMutex;

    const std::string xsetWacomExecutable;

    std::string runXSetWacom(const std::vector<std::string>& args);

    std::vector<std::string> buildArgv(const std::vector<std::string>&);
    std::unique_ptr<const char*> argvToPointers(const std::vector<std::string>&);

    std::pair<int, int> getDimensions(const std::string&);

public:
    XSetWacomShell(const std::string&);

    double getMainDeviceAspectRatio(const std::string&);
    void setMapToOutput(const std::string&, Rectangle*);

    virtual ~XSetWacomShell() {}
};

//error classes
class XSetWacomShellError : public MapToOutputError
{
public:
    XSetWacomShellError(const std::string& x)
        : MapToOutputError(x)
    {}
};
