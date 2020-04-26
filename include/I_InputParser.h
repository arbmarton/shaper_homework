#pragma once

#include "Enums.h"

#include <filesystem>
#include <string>

namespace shaper {

class Converter;

class I_InputParser
{
public:
    I_InputParser(Converter* parentConverter, const std::filesystem::path& input, const SupportedInputFormats format);
    virtual ~I_InputParser() = default;

    virtual bool parse() = 0;

protected:
    Converter* m_parentConverter;
    std::filesystem::path m_inputFile;

    SupportedInputFormats m_format;
};

}  // namespace shaper