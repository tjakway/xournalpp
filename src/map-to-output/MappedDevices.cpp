#include "map-to-output/MappedDevices.h"
#include "map-to-output/MapToOutputError.h"

#include <sstream>
#include <regex>

#include <memory>

//for g_warning
#include <glib.h>


namespace {
    //see https://stackoverflow.com/questions/6444842/efficient-way-to-check-if-stdstring-has-only-spaces
    bool stringIsNonWhitespace(const std::string& str)
    {
        return str.find_first_not_of(' ') != std::string::npos;
    }

    std::vector<std::string> stringToLines(const std::string& str)
    {
        std::vector<std::string> lines;

        //see https://stackoverflow.com/questions/13172158/c-split-string-by-line
        std::stringstream ss(str);
        std::string thisLine;
        while(std::getline(ss, thisLine, '\n'))
        {
            if(stringIsNonWhitespace(thisLine))
            {
                lines.emplace_back(thisLine);
            }
        }

        return lines;
    }

    std::vector<std::regex> stringsToRegexes(std::vector<std::string> strs)
    {
        std::vector<std::regex> regexes;
        for(const std::string& thisStr : strs)
        {
            regexes.emplace_back(thisStr);
        }

        return regexes;
    }

    MappedDevices::MappedDeviceSet getMatches(
                                        const std::string& mainDeviceRegexStr,
                                        std::vector<std::regex> regexes,
                                        std::vector<std::string> lines)
    {
        const std::regex mainDeviceRegex(mainDeviceRegexStr);

        std::unique_ptr<std::string> mainDevice;
        std::unordered_set<std::string> matchingLines;

        for(const std::string& thisLine : lines)
        {
            //check if we should map the device with this name
            for(const std::regex& thisRegex : regexes)
            {
                std::smatch m;
                if(std::regex_search(thisLine, m, thisRegex))
                {
                    matchingLines.emplace(thisLine);
                }
            }


            //check if this is the main device
            std::smatch n;
            if(std::regex_search(thisLine, n, mainDeviceRegex))
            {
                //make sure there's only 1 main device
                if(mainDevice)
                {
                    std::ostringstream ss;
                    ss << "Too many main devices, " <<
                        thisLine << " matches main device regex " <<
                        mainDeviceRegexStr << 
                        " but already found main device named" <<
                        *mainDevice;
                    throw TooManyMainDevices(ss.str());
                }
                else
                {
                    mainDevice = std::unique_ptr<std::string>(new std::string(thisLine));
                }
            }
        }

        //make sure we found a main device
        if(mainDevice)
        {
            return MappedDeviceSet(matchingLines, *mainDevice);
        }
        else
        {
            std::ostringstream ss;
            ss << "No device found matching main device regex " <<
                mainDeviceRegexStr;
            throw NoMainDeviceError(ss.str());
        }
    }

    std::string warnIfDeviceListIsEmpty(const std::string& deviceList)
    {
        if(!stringIsNonWhitespace(deviceList))
        {
            g_warning("xsetwacom device list is empty, "
                    "no device regexes will match");
        }
        return deviceList;
    }
}



MappedDevices::MappedDevices(
        const MapToOutputConfig& config,
        const std::string& printedDeviceList)
    : MappedDevices(config.deviceRegexes, printedDeviceList)
{}


MappedDevices::MappedDevices(
        const MapToOutputConfig::DeviceRegexes& deviceRegexes,
        const std::string& printedDeviceList)
    : mappedDevices(getMatches(stringsToRegexes(deviceRegexes),
                 stringToLines(warnIfDeviceListIsEmpty(printedDeviceList))))
{}
