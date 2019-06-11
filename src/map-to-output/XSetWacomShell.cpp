#include "map-to-output/XSetWacomShell.h"

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
            }
        }
    };

    class ArgvWrapper
    {
        char** argv;
        const std::vector<std::string>::size_type len;

    public:
        ArgvWrapper(const std::vector<std::string>& argvVector)
            : len(argvVector.size())
        {
            argv = new char*[len + 1];
            //don't forget to null-terminate the array
            argv[len] = nullptr;
            assert(strlen(*argv) == len);


            for(std::vector<std::string>::size_type i = 0; 
                    i < argvVector.size(); i++)
            {
                const std::string& thisString = argvVector[i];
                argv[i] = new char[thisString.size() + 1];
                memcpy(argv[i], thisString.c_str(), thisString.size());
                argv[i][len] = '\0';
            }
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
            for(std::vector<std::string>::size_type i = 0; i < len; i++)
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
    argv.emplace_back(config->xsetWacomExecutable);
    for(const auto& x : args)
    {
        argv.emplace_back(x);
    }

    return argv;
}


std::string XSetWacomShell::runXSetWacom(
            const std::vector<std::string>& args)
{ 

    //need argvVector to be in scope while argv is active
    std::vector<std::string> argvVector = buildArgv(args);
    ArgvWrapper argv(argvVector);



    GError* _launchError;
    gint exitStatus = 0;


    char *_stdoutBuf, *_stderrBuf;
    const auto res = g_spawn_sync(
        nullptr, //working directory
        argv.getArgv(),
        nullptr, //inherit parent environment
        G_SPAWN_SEARCH_PATH,
        nullptr, //no setup function
        nullptr, //no setup function args
        &_stdoutBuf,
        &_stderrBuf,
        &exitStatus,
        &_launchError);

    std::unique_ptr<char> stdoutBuf(_stdoutBuf),
        stderrBuf(_stderrBuf);

    std::unique_ptr<GError, GErrorDeleter> launchError(_launchError);

    //format and throw exception
    const auto err = [&stdoutBuf, &stderrBuf, &exitStatus, &argvVector]
        (const GError& gError) {

        std::ostringstream ss;
        ss << "Error running `";
        for(const auto& thisArg : argvVector)
        {
            ss << "thisArg " << thisArg;
        }
        //just ignore the dangling space
        ss << "`:" << std::endl
           << "\tglib error message: " << gError.message << std::endl
           << "\texit code: " << exitStatus << std::endl
           << "\tstdout: " << *stdoutBuf << std::endl
           << "\tstderr: " << *stderrBuf;

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
        GError* _checkExitError;
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

    vector<string> tokens{istream_iterator<string>{iss},
                          istream_iterator<string>{}};

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

    return static_cast<double(w) / static_cast<double>(h);
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
