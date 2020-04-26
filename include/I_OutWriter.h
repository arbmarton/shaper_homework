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
    Converter* m_parentConverter;
    std::filesystem::path m_filePath;

    SupportedOutputFormats m_format;
};

}  // namespace shaper