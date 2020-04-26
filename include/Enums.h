#pragma once

#include <string>
#include <stdexcept>

namespace shaper {

enum class SupportedInputFormats
{
    OBJ
};

inline SupportedInputFormats stringToInputFormat(const std::string& str)
{
    if (str == "obj")
    {
        return SupportedInputFormats::OBJ;
    }
    else
    {
        throw std::invalid_argument(str + " is not a valid input format.");
    }
}

enum class SupportedOutputFormats
{
    STL
};

inline SupportedOutputFormats stringToOutputFormat(const std::string& str)
{
    if (str == "stl")
    {
        return SupportedOutputFormats::STL;
    }
    else
    {
        throw std::invalid_argument(str + " is not a valid output format.");
    }
}

}  // namespace shaper