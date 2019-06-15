#include "map-to-output/XSetWacomShell.h"
#include "map-to-output/MapToOutputUtil.h"

#include <sstream>
#include <memory>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>

//for strlen
#include <cstdlib>
//for memcpy
#include <cstring>

#include <cassert>

#include <glib.h>

std::mutex XSetWacomShell::exeMutex {};

namespace {
    //ensure GError is free'd correctly
    class GErrorDeleter
    {
    public:
        void operator()(GError* err)
        {
            if(err != nullptr)
            {
                g_error_free(err);
                err = nullptr;
            }
        }
    };

    class GStringDeleter
    {
    public:
        void operator()(char* str)
        {
            if(str != nullptr)
            {
                g_string_free(str);
                err = nullptr;
            }
        }
    };

    class ArgvWrapper
    {
        char** argv;
        const std::vector<std::string>::size_type len;

        std::vector<std::string> returnFullPathVector(
                const std::vector<std::string>& vec)
        {
            //look up the full path to the passed element
            std::vector<std::string> fullPathVec;
            const std::string fullPath = MapToOutputUtil::findProgramInPath(vec.at(0));

            fullPathVec.emplace_back(fullPath);
            //skip the first element
            for(auto it = vec.begin()+1; it != vec.end(); ++it)
            {
                fullPathVec.emplace_back(*it);
            }

            return fullPathVec;
        }

    public:
        ArgvWrapper(const std::vector<std::string>& passedVec)
            : len(passedVec.size())
        {
            const std::vector<std::string> argvVector = 
                returnFullPathVector(passedVec);

            argv = new char*[len];

            for(std::vector<std::string>::size_type i = 0; 
                    i < argvVector.size(); i++)
            {
                const auto len = argvVector[i].size();
                //allocate and null terminate string
                char* dest = new char[len + 1];
                dest[len] = '\0';

                //copy the string
                void* ret = memcpy(dest, argvVector[i].c_str(), len);
                assert(ret == dest);

                //add it to argv
                argv[i] = dest;
            }
            assert(argv != nullptr);
        }

        char** getArgv() const
        {
            return argv;
        }

        std::vector<std::string>::size_type size() const
        {
            return len;
        }

        virtual ~ArgvWrapper()
        {
            for(unsigned int i = 0; i < size(); i++)
            {
                delete[] argv[i];
            }
            delete[] argv;
        }

    };
}

std::vector<std::string> XSetWacomShell::buildArgv(const std::vector<std::string>& args)
{
    //insert the executable name at the front and copy in args
    std::vector<std::string> argv;
    argv.emplace_back(xsetWacomExecutable);
    for(const auto& x : args)
    {
        argv.emplace_back(x);
    }

    return argv;
}


std::string XSetWacomShell::runXSetWacom(
            const std::vector<std::string>& args)
{ 
    std::lock_guard<std::mutex> {exeMutex};

    std::vector<std::string> argvVector = buildArgv(args);
    ArgvWrapper argv(argvVector);



    GError* _launchError = nullptr;
    gint exitStatus = 0;


    char *_stdoutBuf, *_stderrBuf;
    const auto res = g_spawn_sync(
        nullptr, //working directory
        argv.getArgv(),
        nullptr, //inherit parent environment
        G_SPAWN_DEFAULT,
        nullptr, //no setup function
        nullptr, //no setup function args
        &_stdoutBuf,
        &_stderrBuf,
        &exitStatus,
        &_launchError);

    std::unique_ptr<char, GStringDeleter> stdoutBuf(_stdoutBuf),
        stderrBuf(_stderrBuf);

    std::unique_ptr<GError, GErrorDeleter> launchError(_launchError);

    const auto getStdout = [&stdoutBuf](){
        if(stdoutBuf) {
            return std::string(stdoutBuf.get());
        } else {
            return std::string();
        }
    };

    const auto getStderr = [&stderrBuf](){
        if(stderrBuf) {
            return std::string(stderrBuf.get());
        } else {
            return std::string();
        }
    };

    //format and throw exception
    const auto err = [&getStdout, &getStderr, &exitStatus, &argvVector]
        (const GError& gError) {

        std::ostringstream ss;
        ss << "Error running `";
        for(const auto& thisArg : argvVector)
        {
            ss << thisArg << " ";
        }
        //just ignore the dangling space
        ss << "`:" << std::endl
           << "\tglib error message: " << gError.message << std::endl
           << "\texit code: " << exitStatus << std::endl
           << "\tstdout: " << getStdout() << std::endl
           << "\tstderr: " << getStderr();

        throw XSetWacomShellError(ss.str());
    };

    if(!res)
    {
        err(*launchError);
        //silence return warnings
        return nullptr;
    }
    else
    {
        GError* _checkExitError = nullptr;
        const auto exitRes = g_spawn_check_exit_status(exitStatus, &_checkExitError);

        std::unique_ptr<GError, GErrorDeleter> checkExitError(_checkExitError);

        if(!exitRes)
        {
            err(*checkExitError);
            return nullptr;
        }
        else
        {
            return std::string(stdoutBuf.get());
        }
    }

}


std::pair<int, int> XSetWacomShell::getDimensions(
        const std::string& deviceName)
{
    const std::vector<std::string> args {
        "--get", deviceName, "Area"
    };

    const std::string stdoutRes = runXSetWacom(args);
    std::istringstream iss(stdoutRes);

    //from https://stackoverflow.com/a/237280/389943
    std::vector<string> tokens{std::istream_iterator<string>{iss},
                               std::istream_iterator<string>{}};

    const unsigned int expectedTokens = 4;
    if(tokens.size() != expectedTokens)
    {
        std::ostringstream ss;
        ss << "Error running xsetwacom with args `";
        for(const auto& x : args)
        {
            ss << x << " ";
        }
        ss << "` " << "expected " << expectedTokens << " in stdout"
            << " but got " << tokens.size();

        throw XSetWacomShellError(ss.str());
    }
    else
    {
        int w = stoi(tokens.at(2));
        int h = stoi(tokens.at(3));

        assert(w > 0);
        assert(h > 0);

        return std::make_pair(w, h);
    }
}


double XSetWacomShell::getMainDeviceAspectRatio(const std::string& deviceName)
{
    int w, h;
    std::tie(w, h) = getDimensions(deviceName);

    return static_cast<double>(w) / static_cast<double>(h);
}

void XSetWacomShell::setMapToOutput(
        const std::string& deviceName,
        Rectangle* rect)
{
    std::vector<std::string> args {
        "--set", deviceName, "MapToOutput"
    };
    if(rect)
    {
        std::ostringstream formatRect;
        formatRect << rect->width << "+"
            << rect->height << "+"
            << rect->x << "+"
            << rect->y;
        args.emplace_back(formatRect.str());
    }
    else
    {
        args.emplace_back("desktop");
    }

    runXSetWacom(args);
}

std::string XSetWacomShell::getDevices()
{
    //run the xsetwacom command
    return runXSetWacom(std::vector<std::string>{ "--list", "--devices" });
}


std::vector<std::string> XSetWacomShell::getDeviceLines()
{
    const std::string stdoutRes = getDevices();

    //divide the output into lines
    std::vector<std::string> lines;
    std::string line;
    std::istringstream iss {stdoutRes};

    while(std::getline(iss, line))
    {
        //insert nonempty lines
        if(MapToOutputUtil::stringIsNonWhitespace(line))
        {
            lines.emplace_back(line);
        }
    }

    return lines;
}


XSetWacomShell::XSetWacomShell(const std::string& _xsetWacomExecutable)
    : xsetWacomExecutable(_xsetWacomExecutable)
{}
