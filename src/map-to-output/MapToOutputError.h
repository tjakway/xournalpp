#pragma once

#include <string>
#include <stdexcept>

class MapToOutputError : public std::runtime_error
{
public:
    MapToOutputError(const std::string& what)
        : std::runtime_error(what)
    {}

    virtual ~MapToOutputError() {}
};
