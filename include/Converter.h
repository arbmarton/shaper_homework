#pragma once

#include "Enums.h"
#include "Triangle.h"

#pragma warning(push)
#pragma warning(disable : 4201)
#include "glm.hpp"
#pragma warning(pop)


#include <string>
#include <optional>
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

    const std::string& getError() const
    {
        return error;
    }

    // Returns nullopt if succesful
    std::optional<std::string> convert();

    std::vector<glm::vec4>& getVertices()
    {
        return vertices;
    }

    const std::vector<glm::vec4>& getVertices() const
    {
        return vertices;
    }

    std::vector<glm::vec3>& getTextureVertices()
    {
        return textureVertices;
    }

    const std::vector<glm::vec3>& getTextureVertices() const
    {
        return textureVertices;
    }

    std::vector<glm::vec3>& getVertexNormals()
    {
        return vertexNormals;
    }

    const std::vector<glm::vec3>& getVertexNormals() const
    {
        return vertexNormals;
    }

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

    // DATA members like points and faces and stuff here
    std::vector<glm::vec4> vertices;
    std::vector<glm::vec3> textureVertices;
    std::vector<glm::vec3> vertexNormals;

    std::vector<Triangle> triangles;
};

}  // namespace shaper