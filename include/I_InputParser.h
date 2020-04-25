#pragma once

#include "Enums.h"

#include <filesystem>

namespace shaper {

class Converter;

class I_InputParser
{
public:
    I_InputParser(Converter* parentConverter, const std::filesystem::path& input, const SupportedInputFormats format);
    virtual ~I_InputParser() = default;

    virtual void parse() = 0;

protected:
    Converter* parentConverter;
    std::filesystem::path inputFile;

    SupportedInputFormats format;
};

}  // namespace shaper