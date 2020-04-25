#include "ObjParser.h"
#include "Utilities.h"
#include "Converter.h"

#pragma warning(push)
#pragma warning(disable : 4201)
#include "glm.hpp"
#pragma warning(pop)

#include <iostream>
#include <array>

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
            const auto error = parseFaceLine(splitLine);
            if (error)
            {
                return error;
            }
        }
        else
        {
            std::cout << "PARSING: Ignoring token \"" << token << "\"\n";
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

std::optional<std::string> ObjParser::parseFaceLine(const std::vector<std::string>& vec)
{
    if (vec.size() < 4)
    {
        return "Face parsing: line is too short\n";
    }

    std::vector<Triangle> trianglesInFace;
    trianglesInFace.resize(vec.size() - 3);

    // Check if its a single number, the parsing is more simple then
    if (vec[1].find('/') == std::string::npos)
    {
        std::vector<int> numbers;
        numbers.reserve(vec.size() - 1);
        for (int i = 1; i < vec.size(); ++i)
        {
            numbers.push_back(std::stoi(vec[i]));
        }

        for (int i = 0; i < trianglesInFace.size(); ++i)
        {
            glm::vec3 a;
            glm::vec3 b;
            glm::vec3 c;

            if (numbers[i + 0] < 0)
            {
                a = parentConverter->getVertices()[parentConverter->getVertices().size() + numbers[i + 0]];
            }
            else
            {
                a = parentConverter->getVertices()[numbers[i + 0] - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
            }

            if (numbers[i + 1] < 0)
            {
                b = parentConverter->getVertices()[parentConverter->getVertices().size() + numbers[i + 1]];
            }
            else
            {
                b = parentConverter->getVertices()[numbers[i + 1] - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
            }

            if (numbers[i + 2] < 0)
            {
                c = parentConverter->getVertices()[parentConverter->getVertices().size() + numbers[i + 2]];
            }
            else
            {
                c = parentConverter->getVertices()[numbers[i + 2] - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
            }

            trianglesInFace[i] = { a, b, c, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } ,{ 0, 0, 0 } };
        }
    }
    else
    {
        // Texture coords are missing
        if (utilities::splitString(vec[1], '/')[1] == "")
        {
            std::vector<std::pair<int, int>> numbers; // vertices and normals
            numbers.reserve(vec.size() - 1);
            for (int i = 1; i < vec.size(); ++i)
            {
                const std::vector<std::string> splitGroup{ utilities::splitString(vec[i], '/') };
                numbers.push_back(std::make_pair<int, int>(std::stoi(splitGroup[0]), std::stoi(splitGroup[2])));
            }

            for (int i = 0; i < trianglesInFace.size(); ++i)
            {
                glm::vec3 a;
                glm::vec3 b;
                glm::vec3 c;
                glm::vec3 normalA;
                glm::vec3 normalB;
                glm::vec3 normalC;

                // VERTICES
                if (numbers[i + 0].first < 0)
                {
                    a = parentConverter->getVertices()[parentConverter->getVertices().size() + numbers[i + 0].first];
                }
                else
                {
                    a = parentConverter->getVertices()[numbers[i + 0].first - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                if (numbers[i + 1].first < 0)
                {
                    b = parentConverter->getVertices()[parentConverter->getVertices().size() + numbers[i + 1].first];
                }
                else
                {
                    b = parentConverter->getVertices()[numbers[i + 1].first - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                if (numbers[i + 2].first < 0)
                {
                    c = parentConverter->getVertices()[parentConverter->getVertices().size() + numbers[i + 2].first];
                }
                else
                {
                    c = parentConverter->getVertices()[numbers[i + 2].first - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }


                // NORMALS
                if (numbers[i + 0].second < 0)
                {
                    normalA = parentConverter->getVertexNormals()[parentConverter->getVertexNormals().size() + numbers[i + 0].second];
                }
                else
                {
                    normalA = parentConverter->getVertexNormals()[numbers[i + 0].second - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                if (numbers[i + 1].second < 0)
                {
                    normalB = parentConverter->getVertexNormals()[parentConverter->getVertexNormals().size() + numbers[i + 1].second];
                }
                else
                {
                    normalB = parentConverter->getVertexNormals()[numbers[i + 1].second - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                if (numbers[i + 2].second < 0)
                {
                    normalC = parentConverter->getVertexNormals()[parentConverter->getVertexNormals().size() + numbers[i + 2].second];
                }
                else
                {
                    normalC = parentConverter->getVertexNormals()[numbers[i + 2].second - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                trianglesInFace[i] = { a, b, c, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, normalA, normalB, normalC };
            }
        }
        else // Everything is present
        {
            std::vector<std::array<int, 3>> numbers; // vertices and normals
            numbers.reserve(vec.size() - 1);
            for (int i = 1; i < vec.size(); ++i)
            {
                const std::vector<std::string> splitGroup{ utilities::splitString(vec[i], '/') };
                numbers.push_back({ std::stoi(splitGroup[0]), std::stoi(splitGroup[1]), std::stoi(splitGroup[2]) });
            }

            for (int i = 0; i < trianglesInFace.size(); ++i)
            {
                glm::vec3 a;
                glm::vec3 b;
                glm::vec3 c;
                glm::vec3 textureA;
                glm::vec3 textureB;
                glm::vec3 textureC;
                glm::vec3 normalA;
                glm::vec3 normalB;
                glm::vec3 normalC;

                // VERTICES
                if (numbers[i + 0][0] < 0)
                {
                    a = parentConverter->getVertices()[parentConverter->getVertices().size() + numbers[i + 0][0]];
                }
                else
                {
                    a = parentConverter->getVertices()[numbers[i + 0][0] - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                if (numbers[i + 1][0] < 0)
                {
                    b = parentConverter->getVertices()[parentConverter->getVertices().size() + numbers[i + 1][0]];
                }
                else
                {
                    b = parentConverter->getVertices()[numbers[i + 1][0] - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                if (numbers[i + 2][0] < 0)
                {
                    c = parentConverter->getVertices()[parentConverter->getVertices().size() + numbers[i + 2][0]];
                }
                else
                {
                    c = parentConverter->getVertices()[numbers[i + 2][0] - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                // TEXTURES
                if (numbers[i + 0][1] < 0)
                {
                    textureA = parentConverter->getTextureVertices()[parentConverter->getTextureVertices().size() + numbers[i + 0][1]];
                }
                else
                {
                    textureA = parentConverter->getTextureVertices()[numbers[i + 0][1] - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                if (numbers[i + 1][1] < 0)
                {
                    textureB = parentConverter->getTextureVertices()[parentConverter->getTextureVertices().size() + numbers[i + 1][1]];
                }
                else
                {
                    textureB = parentConverter->getTextureVertices()[numbers[i + 1][1] - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                if (numbers[i + 2][1] < 0)
                {
                    textureC = parentConverter->getTextureVertices()[parentConverter->getTextureVertices().size() + numbers[i + 2][1]];
                }
                else
                {
                    textureC = parentConverter->getTextureVertices()[numbers[i + 2][1] - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }


                // NORMALS
                if (numbers[i + 0][2] < 0)
                {
                    normalA = parentConverter->getVertexNormals()[parentConverter->getVertexNormals().size() + numbers[i + 0][2]];
                }
                else
                {
                    normalA = parentConverter->getVertexNormals()[numbers[i + 0][2] - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                if (numbers[i + 1][2] < 0)
                {
                    normalB = parentConverter->getVertexNormals()[parentConverter->getVertexNormals().size() + numbers[i + 1][2]];
                }
                else
                {
                    normalB = parentConverter->getVertexNormals()[numbers[i + 1][2] - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                if (numbers[i + 2][2] < 0)
                {
                    normalC = parentConverter->getVertexNormals()[parentConverter->getVertexNormals().size() + numbers[i + 2][2]];
                }
                else
                {
                    normalC = parentConverter->getVertexNormals()[numbers[i + 2][2] - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                trianglesInFace[i] = { a, b, c, textureA, textureB, textureC, normalA, normalB, normalC };
            }
        }
    }

    for (const Triangle& tr : trianglesInFace)
    {
        parentConverter->getTriangles().emplace_back(tr);
    }

    return {};
}

}  // namespace shaper