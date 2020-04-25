#pragma once

#include "Enums.h"

#include <filesystem>

namespace shaper {

class Converter;

class I_OutWriter
{
public:
    I_OutWriter(Converter* parentConverter, const std::filesystem::path& outFile, const SupportedOutputFormats format);
    virtual ~I_OutWriter();

    virtual bool write() = 0;

protected:
    Converter* parentConverter;
    std::filesystem::path filePath;

    SupportedOutputFormats format;
};

}  // namespace shaper