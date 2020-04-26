#include "I_InputParser.h"

namespace shaper {

I_InputParser::I_InputParser(Converter* parentConverter, const std::filesystem::path& input, const SupportedInputFormats format)
    : m_parentConverter(parentConverter)
    , m_inputFile(input)
    , m_format(format)
{
}

}  // namespace shaper