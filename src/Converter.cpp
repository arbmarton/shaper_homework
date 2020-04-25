#include "Converter.h"
#include "ObjParser.h"
#include "StlWriter.h"

namespace shaper {

Converter::Converter(
    const SupportedInputFormats input,
    const std::filesystem::path& inputPath,
    const SupportedOutputFormats output,
    const std::filesystem::path& outputPath)
{
    switch (input)
    {
        case SupportedInputFormats::OBJ:
            parser = std::make_unique<ObjParser>(this, inputPath);
            break;
    }

    switch (output)
    {
        case SupportedOutputFormats::STL:
            writer = std::make_unique<StlWriter>(this, outputPath);
            break;
    }
}

Converter::~Converter() = default;

bool Converter::convert()
{
    bool err = parser->parse();
    if (!err)
    {
        return false;
    }

    err = writer->write();
    if (!err)
    {
        return false;
    }

    return {};
}

}  // namespace shaper