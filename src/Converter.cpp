#include "Converter.h"
#include "ObjParser.h"
#include "StlWriter.h"

#include "gtc/matrix_transform.hpp"

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

void Converter::addRotation(const glm::vec3& v, const float radians)
{
    trf = glm::rotate(trf, radians, v);
}

void Converter::addTranslation(const glm::vec3& v)
{
    trf = glm::translate(trf, v);
}

void Converter::addScaling(const glm::vec3& v)
{
    trf = glm::scale(trf, v);
}

bool Converter::convert()
{
    bool err = parser->parse();
    if (!err)
    {
        return false;
    }

    // Dont bother if its the unit matrix
    if (trf != glm::mat4(1.0f))
    {
        // TODO: this can be parallelized

        for (Triangle& tr : triangles)
        {
            tr.applyTransformation(trf);
        }
    }

    err = writer->write();
    if (!err)
    {
        return false;
    }

    return true;
}

}  // namespace shaper