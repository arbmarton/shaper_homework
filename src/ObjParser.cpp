#include "ObjParser.h"
#include "Utilities.h"
#include "Converter.h"

#pragma warning(push)
#pragma warning(disable : 4201)
#include "glm.hpp"
#pragma warning(pop)

#include <iostream>

namespace shaper {

ObjParser::ObjParser(Converter* parentConverter, const std::filesystem::path& input)
    : I_InputParser(parentConverter, input, SupportedInputFormats::OBJ)
{
}

std::optional<std::string> ObjParser::parse()
{
    const std::vector<std::string> rawLineData{ utilities::readTextFile(inputFile) };

    for (const std::string& str : rawLineData)
    {
        if (str.size() == 0)
        {
            continue;
        }

        const std::vector<std::string> splitLine{ utilities::splitString(str, ' ') };

        if (splitLine.size() == 0)
        {
            continue;
        }

        const std::string token = splitLine[0];
        if (token == "v")
        {
            const auto error = parseVertexLine(splitLine);
            if (error)
            {
                return error;
            }
        }
        else if (token == "vt")
        {
            const auto error = parseTextureVertexLine(splitLine);
            if (error)
            {
                return error;
            }
        }
        else if (token == "vn")
        {
            const auto error = parseVertexNormalLine(splitLine);
            if (error)
            {
                return error;
            }
        }
        else if (token == "f")
        {
        }
        else
        {
            std::cout << "PARSE   Ignoring token " << token << "\n";
        }
    }

    return {};
}

std::optional<std::string> ObjParser::parseVertexLine(const std::vector<std::string>& vec)
{
    if (vec.size() != 4 && vec.size() != 5)
    {
        return "Vertex data parsing: incorrect line size\n";
    }

    glm::vec4 vertex{ 0, 0, 0, 1.0f };
    vertex.x = std::stof(vec[1]);
    vertex.y = std::stof(vec[2]);
    vertex.z = std::stof(vec[3]);

    if (vec.size() == 5)
    {
        vertex.w = std::stof(vec[4]);
    }

    parentConverter->getVertices().emplace_back(vertex);

    return {};
}

std::optional<std::string> ObjParser::parseTextureVertexLine(const std::vector<std::string>& vec)
{
    if (vec.size() < 2 || vec.size() > 4)
    {
        return "Vertex texture data parsing: incorrect line size\n";
    }

    glm::vec3 textureVertex{ 0, 0, 0 };
    textureVertex.x = std::stof(vec[1]);
    if (vec.size() > 2)
    {
        textureVertex.y = std::stof(vec[2]);
    }
    if (vec.size() > 3)
    {
        textureVertex.z = std::stof(vec[3]);
    }

    parentConverter->getTextureVertices().emplace_back(textureVertex);

    return {};
}

std::optional<std::string> ObjParser::parseVertexNormalLine(const std::vector<std::string>& vec)
{
    if (vec.size() != 4)
    {
        return "Vertex normals parsing: incorrect line size\n";
    }

    glm::vec3 vertexNormal{ 0, 0, 0 };

    vertexNormal.x = std::stof(vec[1]);
    vertexNormal.y = std::stof(vec[2]);
    vertexNormal.z = std::stof(vec[3]);

    parentConverter->getVertexNormals().emplace_back(vertexNormal);

    return {};
}

}  // namespace shaper