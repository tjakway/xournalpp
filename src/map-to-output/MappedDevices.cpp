#include "MappedDevices.h"

#include <sstream>
#include <regex>

//for g_warning
#include <glib.h>

namespace {
    //see https://stackoverflow.com/questions/6444842/efficient-way-to-check-if-stdstring-has-only-spaces
    bool stringIsNonWhitespace(const std::string& str)
    {
        return str.find_first_not_of(' ') != std::string::npos;
    }

    vector<std::string> stringToLines(const std::string& str)
    {
        vector<std::string> lines;

        //see https://stackoverflow.com/questions/13172158/c-split-string-by-line
        std::stringstream ss(str);
        std::string thisLine;
        while(std::getline(ss, thisLine, '\n'))
        {
            if(stringIsNonWhitespace(to))
            {
                lines.emplace_back(to);
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

    MappedDeviceSet getMatches(std::vector<std::regex> regexes,
                                        std::vector<std::string> lines)
    {
        MappedDeviceSet matchingLines;

        for(const std::string& thisLine : lines)
        {
            for(const std::regex& thisRegex)
            {
                std::cmatch m;
                if(std::regex_search(thisLine, m, thisRegex))
                {
                    matchingLines.emplace_back(thisLine);
                }
            }
        }

        return matchingLines;
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
        const std::vector<std::string>& deviceRegexes,
        const std::string& printedDeviceList)
    : getMatches(stringsToRegexes(deviceRegexes),
                 stringToLines(warnIfDeviceListIsEmpty(printedDeviceList)))
{}
