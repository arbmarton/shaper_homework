#include "I_OutWriter.h"

namespace shaper {

I_OutWriter::I_OutWriter(Converter* parentConverter, const std::filesystem::path& outFile, const SupportedOutputFormats format)
    : m_parentConverter(parentConverter)
    , m_filePath(outFile)
    , m_format(format)
{
}

I_OutWriter::~I_OutWriter() = default;

}  // namespace shaper