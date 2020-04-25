#include "I_OutWriter.h"

namespace shaper {

I_OutWriter::I_OutWriter(Converter* parentConverter, const std::filesystem::path& outFile, const SupportedOutputFormats format)
	: parentConverter(parentConverter)
	, filePath(outFile)
	, format(format)
{

}

I_OutWriter::~I_OutWriter() = default;

}  // namespace shaper