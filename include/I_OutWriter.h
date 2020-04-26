#pragma once

#include "Enums.h"

#include <filesystem>

namespace shaper {

class Converter;

class I_OutWriter
{
public:
    I_OutWriter(Converter* parentConverter, const std::filesystem::path& outFile, const SupportedOutputFormats format) noexcept;
    virtual ~I_OutWriter();

    I_OutWriter() = delete;
    I_OutWriter(const I_OutWriter& rhs) = delete;
    I_OutWriter(I_OutWriter&& rhs) = delete;

    I_OutWriter& operator=(const I_OutWriter& rhs) = delete;
    I_OutWriter& operator=(I_OutWriter&& rhs) = delete;

    virtual bool write() = 0;

protected:
    Converter* m_parentConverter;
    std::filesystem::path m_filePath;

    SupportedOutputFormats m_format;
};

}  // namespace shaper