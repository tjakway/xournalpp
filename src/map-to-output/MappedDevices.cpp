#include "map-to-output/MappedDevices.h"
#include "map-to-output/MapToOutputError.h"
#include "map-to-output/MapToOutputUtil.h"

#include <sstream>
#include <regex>
#include <algorithm>
#include <memory>

//for g_warning
#include <glib.h>


namespace {
    std::vector<std::string> stringToLines(const std::string& str)
    {
        std::vector<std::string> lines;

        //see https://stackoverflow.com/questions/13172158/c-split-string-by-line
        std::stringstream ss(str);
        std::string thisLine;
        while(std::getline(ss, thisLine, '\n'))
        {
            if(MapToOutputUtil::stringIsNonWhitespace(thisLine))
            {
                lines.emplace_back(thisLine);
            }
        }

        return lines;
    }

    template <typename it>
    std::vector<std::regex> stringsToRegexes(it begin, it end)
    {
        std::vector<std::regex> regexes;
        std::for_each(begin, end, 
                [&regexes](const std::string& thisStr){
                
            //construct the regex in place
            regexes.emplace_back(thisStr);
        });

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
                    throw TooManyMainDevicesError(ss.str());
                }
                else
                {
                    mainDevice = std::unique_ptr<std::string>(new std::string(thisLine));
                    
                    //the main device is implicitly one of the matching devices
                    matchingLines.emplace(thisLine);
                }
            }
        }

        //make sure we found a main device
        if(mainDevice)
        {
            return MappedDevices::MappedDeviceSet{matchingLines, *mainDevice};
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
        if(!MapToOutputUtil::stringIsNonWhitespace(deviceList))
        {
            g_warning("xsetwacom device list is empty, "
                    "no device regexes will match");
        }
        return deviceList;
    }

    std::string extractDeviceName(const std::string& thisLine)
    {
        static const std::string deviceNameFieldRegexStr = 
            R"RAW(^\s*([\w\d]\s+([\w\d]+)?|[\w\d])+(?=\s+id: \d+\s+type:\s+\w+))RAW";
        static const std::regex deviceNameFieldRegex(deviceNameFieldRegexStr);

        //see https://www.regular-expressions.info/stdregex.html
        std::smatch match;
        if(std::regex_search(thisLine, match, deviceNameFieldRegex) && match.size() > 1)
        {
            return match.str(0);
        }
        else
        {
            std::ostringstream ss;
            ss << "Could not find match for regex << " <<
                deviceNameFieldRegexStr << 
                " >> in device name " << thisLine;

            throw DeviceNameExtractionError(ss.str());
        }
    }

    std::vector<std::string> extractDeviceNames(
            const std::vector<std::string>& lines)
    {
        std::vector<std::string> extractedNames;

        for(const auto& i : lines)
        {
            extractedNames.emplace_back(extractDeviceName(i));
        }

        return extractedNames;
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
    : mappedDevices(
            getMatches(deviceRegexes.mainDevice,
                stringsToRegexes(deviceRegexes.deviceRegexes.cbegin(), 
                    deviceRegexes.deviceRegexes.cend()),
            stringToLines(warnIfDeviceListIsEmpty(printedDeviceList))))
{}

MappedDevices::MappedDevices(const MappedDevices& other)
    : mappedDevices(other.getMappedDevices())
{}


MappedDevices::MappedDeviceSet MappedDevices::getMappedDevices() const
{
    return mappedDevices;
}
