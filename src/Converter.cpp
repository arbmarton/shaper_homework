#include "Converter.h"
#include "ObjParser.h"
#include "StlWriter.h"
#include "Ray.h"

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
            m_parser = std::make_unique<ObjParser>(this, inputPath);
            break;
    }

    switch (output)
    {
        case SupportedOutputFormats::STL:
            m_writer = std::make_unique<StlWriter>(this, outputPath);
            break;
    }
}

Converter::~Converter() = default;

void Converter::addRotation(const glm::vec3& v, const float radians)
{
    m_trf = glm::rotate(m_trf, radians, v);
}

void Converter::addTranslation(const glm::vec3& v)
{
    m_trf = glm::translate(m_trf, v);
}

void Converter::addScaling(const glm::vec3& v)
{
    m_trf = glm::scale(m_trf, v);
}

float Converter::calculateMeshArea() const
{
    // TODO: could be parallelized, make accum atomic
    float accum = 0.0f;

    for (const Triangle& tr : m_triangles)
    {
        accum += tr.area();
    }

    return accum;
}

float Converter::calculateMeshVolume() const
{
    // TODO: could be parallelized, make accum atomic
    float accum = 0.0f;

    for (const Triangle& tr : m_triangles)
    {
        accum += tr.calculateSignedVolume();
    }

    return abs(accum);
}

// Create a dummy ray, and if it intersects the object an odd number of times its inside
bool Converter::isPointInsideTheMesh(const glm::vec3& pt) const
{
    uint32_t intersections{ 0 };

    const Ray ray{ pt, { 1, 0, 0 } };

    for (const Triangle& tr : m_triangles)
    {
        if (tr.intersectsWith(ray))
        {
            intersections++;
        }
    }

    if (intersections % 2 == 0)
    {
        return false;
    }

    return true;
}

bool Converter::convert()
{
    if (!m_parser->parse())
    {
        return false;
    }

    // Dont bother if its the unit matrix
    if (m_trf != glm::mat4(1.0f))
    {
        // TODO: this can be parallelized

        for (Triangle& tr : m_triangles)
        {
            tr.applyTransformation(m_trf);
        }
    }

    if (!m_writer->write())
    {
        return false;
    }

    return true;
}

}  // namespace shaper