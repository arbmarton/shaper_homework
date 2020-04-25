#include "ObjParser.h"

namespace shaper {

ObjParser::ObjParser(Converter* parentConverter, const std::filesystem::path& input)
    : I_InputParser(parentConverter, input, SupportedInputFormats::OBJ)
{
}

void ObjParser::parse()
{
}

}  // namespace shaper