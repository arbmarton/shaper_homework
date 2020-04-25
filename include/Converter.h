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
        const std::filesystem::path& outputPath);
    Converter() = delete;
    Converter(const Converter& rhs) = delete;
    Converter(Converter&& rhs) = delete;
    ~Converter();

    // Rotate around 'v', by 'radians' amount
    void addRotation(const glm::vec3& v, const float radians);
    void addTranslation(const glm::vec3& v);
    void addScaling(const glm::vec3& v);

    const std::string& getError() const
    {
        return error;
    }

    void setError(const std::string& errorStr)
    {
        error = errorStr;
    }

    bool convert();

    std::vector<Triangle>& getTriangles()
    {
        return triangles;
    }

    const std::vector<Triangle>& getTriangles() const
    {
        return triangles;
    }

private:
    std::string error;

    std::unique_ptr<I_InputParser> parser;
    std::unique_ptr<I_OutWriter> writer;

    std::vector<Triangle> triangles;

    glm::mat4 trf = glm::mat4(1.0f);
};

}  // namespace shaper