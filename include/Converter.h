#pragma once

#include "Enums.h"
#include "Triangle.h"

#pragma warning(push)
#pragma warning(disable : 4201)
#include "glm.hpp"
#pragma warning(pop)

#include <string>
#include <filesystem>
#include <memory>

namespace shaper {

class I_InputParser;
class I_OutWriter;

class Converter
{
public:
    Converter(
        const SupportedInputFormats input,
        const std::filesystem::path& inputPath,
        const SupportedOutputFormats output,
        const std::filesystem::path& outputPath,
        const bool logging = false);
    Converter() = delete;
    Converter(const Converter& rhs) = delete;
    Converter(Converter&& rhs) = delete;
    ~Converter();

    Converter& operator=(const Converter& rhs) = delete;
    Converter& operator=(Converter&& rhs) = delete;

    // Rotate around 'v', by 'radians' amount
    void addRotation(const glm::vec3& v, const float radians);
    void addTranslation(const glm::vec3& v);
    void addScaling(const glm::vec3& v);

    float calculateMeshArea() const;
    float calculateMeshVolume() const;
    bool isPointInsideTheMesh(const glm::vec3& pt) const;

    const std::string& getError() const
    {
        return m_error;
    }

    void setError(const std::string& errorStr)
    {
        m_error = errorStr;
    }

    bool convert();

    std::vector<Triangle>& getTriangles()
    {
        return m_triangles;
    }

    const std::vector<Triangle>& getTriangles() const
    {
        return m_triangles;
    }

    bool isLoggingEnabled() const
    {
        return m_logging;
    }

private:
    // Stores the last encountered error
    std::string m_error;

    std::unique_ptr<I_InputParser> m_parser;
    std::unique_ptr<I_OutWriter> m_writer;

    // The triangles in the mesh
    std::vector<Triangle> m_triangles;

    // The transformation to be applied on the mesh
    glm::mat4 m_trf = glm::mat4(1.0f);

    bool m_logging;
};

}  // namespace shaper