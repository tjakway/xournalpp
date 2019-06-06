#include "map-to-output/XSetWacomShell.h"

#include <sstream>
#include <memory>
#include <string>

#include <glib/glib.h>

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


    //do this instead of allocating GError on the stack in
    //case g_error_new has to do any kind of setup
    std::unique_ptr<GError, GErrorDeleter> newGErrorPointer()
    {
        return std::unique_ptr<GError, GErrorDeleter>(g_error_new());
    }
}

//16384 bytes should be plenty
static const unsigned int outputStreamBufSize = 2^14

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



std::unique_ptr<char*> XSetWacomShell::argvToPointers(const std::vector<std::string>& argvVector)
{
    //turn our vector of arguments into a char**
    std::unique_ptr<char*> argv(new char[argvVector.size()]);
    for(std::vector<std::string>::size_t i = 0; i < argvVector.size(); i++)
    {
        argv[i] = argvVector[i].c_str();
    }
    return argv;
}

std::string XSetWacomShell::runXSetWacom(
            const std::vector<std::string>& args)
{ 

    std::unique_ptr<GError, GErrorDeleter> launchError = newGErrorPointer();

    std::unique_ptr<char> stdoutBuf, stderrBuf; 
    Gint exitStatus = 0;

    //need argvVector to be in scope while argv is active
    std::vector<std::string> argvVector = buildArgv(args);
    std::unique_ptr<char*> argv = argvToPointers(argvVector);

    //format and throw exception
    const auto err = [&stdoutBuf, &stderrBuf, &exitStatus, &argvVector]
        (const GError& gError) {

        std::ostringstream ss;
        ss << "Error running `";
        for(const auto& thisArg : argvVector)
        {
            ss << "thisArg ";
        }
        //just ignore the dangling space
        ss << "`:" << std::endl
           << "\tglib error message: " << gError.message << std::endl
           << "\texit code: " << exitStatus << std::endl
           << "\tstdout: " << *stdoutBuf << std::endl
           << "\tstderr: " << *stderrBuf;

        throw XSetWacomShellError(ss.str());
    };



    if(!g_spawn_sync(
        nullptr, //working directory
        argv.get(),
        nullptr, //inherit parent environment
        G_SPAWN_SEARCH_PATH,
        nullptr, //no setup function
        nullptr, //no setup function args
        &stdoutBuf.get(),
        &stderrBuf.get(),
        &exitStatus,
        &gError.get()))
    {
        err(*gError);
        //silence return warnings
        return nullptr;
    }
    else
    {
        //make sure the process exited successfully before returning
        auto exitErrorPtr = newGErrorPointer();
        if(!g_spawn_check_exit_status(exitStatus, &exitErrorPtr.get()))
        {
            err(*exitErrorPtr);
            return nullptr;
        }
        else
        {
            return std::string(stdoutBuf.get());
        }
    }

}


std::pair<std::string, std::string> XSetWacomShell::getDimensions(
        const std::string& deviceName)
{

}
