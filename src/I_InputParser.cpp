#include "I_InputParser.h"

namespace shaper {

I_InputParser::I_InputParser(Converter* parentConverter, const std::filesystem::path& input, const SupportedInputFormats format)
    : parentConverter(parentConverter)
    , inputFile(input)
    , format(format)
{
}

}  // namespace shaper