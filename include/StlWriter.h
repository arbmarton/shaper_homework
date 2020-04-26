#pragma once

#include "I_OutWriter.h"

namespace shaper {
class StlWriter : public I_OutWriter
{
public:
    StlWriter(Converter* parentConverter, const std::filesystem::path& outFile) noexcept;
    ~StlWriter() = default;

    StlWriter() = delete;
    StlWriter(const StlWriter& rhs) = delete;
    StlWriter(StlWriter&& rhs) = delete;

    StlWriter& operator=(const StlWriter& rhs) = delete;
    StlWriter& operator=(StlWriter&& rhs) = delete;

    virtual bool write() override;
};
}  // namespace shaper