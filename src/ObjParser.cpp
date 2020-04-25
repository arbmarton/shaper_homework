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

bool ObjParser::parse()
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
            if (!parseVertexLine(splitLine))
            {
                return false;
            }
        }
        else if (token == "vt")
        {
            if (!parseTextureVertexLine(splitLine))
            {
                return false;
            }
        }
        else if (token == "vn")
        {
            if (!parseVertexNormalLine(splitLine))
            {
                return false;
            }
        }
        else if (token == "f")
        {
            if (!parseFaceLine(splitLine))
            {
                return false;
            }
        }
        else
        {
            std::cout << "PARSING: Ignoring token \"" << token << "\"\n";
        }
    }

    return true;
}

bool ObjParser::parseVertexLine(const std::vector<std::string>& vec)
{
    if (vec.size() != 4 && vec.size() != 5)
    {
        parentConverter->setError("Vertex data parsing: incorrect line size");
        return false;
    }

    glm::vec4 vertex{ 0, 0, 0, 1.0f };
    vertex.x = std::stof(vec[1]);
    vertex.y = std::stof(vec[2]);
    vertex.z = std::stof(vec[3]);

    if (vec.size() == 5)
    {
        vertex.w = std::stof(vec[4]);
    }

    vertices.emplace_back(vertex);

    return true;
}

bool ObjParser::parseTextureVertexLine(const std::vector<std::string>& vec)
{
    if (vec.size() < 2 || vec.size() > 4)
    {
        parentConverter->setError("Vertex texture data parsing: incorrect line size");
        return false;
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

    textureVertices.emplace_back(textureVertex);

    return true;
}

bool ObjParser::parseVertexNormalLine(const std::vector<std::string>& vec)
{
    if (vec.size() != 4)
    {
        parentConverter->setError("Vertex normals parsing: incorrect line size");
        return false;
    }

    glm::vec3 vertexNormal{ 0, 0, 0 };

    vertexNormal.x = std::stof(vec[1]);
    vertexNormal.y = std::stof(vec[2]);
    vertexNormal.z = std::stof(vec[3]);

    vertexNormals.emplace_back(vertexNormal);

    return true;
}

bool ObjParser::parseFaceLine(const std::vector<std::string>& vec)
{
    if (vec.size() < 4)
    {
        parentConverter->setError("Face parsing: line is too short");
        return false;
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

            if (numbers[0] < 0)
            {
                a = vertices[vertices.size() + numbers[0]];
            }
            else
            {
                a = vertices[numbers[0] - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
            }

            if (numbers[i + 1] < 0)
            {
                b = vertices[vertices.size() + numbers[i + 1]];
            }
            else
            {
                b = vertices[numbers[i + 1] - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
            }

            if (numbers[i + 2] < 0)
            {
                c = vertices[vertices.size() + numbers[i + 2]];
            }
            else
            {
                c = vertices[numbers[i + 2] - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
            }

            trianglesInFace[i] = { a, b, c, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };
        }
    }
    else
    {
        // Texture coords are missing
        if (utilities::splitString(vec[1], '/')[1] == "")
        {
            std::vector<std::pair<int, int>> numbers;  // vertices and normals
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
                if (numbers[0].first < 0)
                {
                    a = vertices[vertices.size() + numbers[0].first];
                }
                else
                {
                    a = vertices[numbers[0].first - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                if (numbers[i + 1].first < 0)
                {
                    b = vertices[vertices.size() + numbers[i + 1].first];
                }
                else
                {
                    b = vertices[numbers[i + 1].first - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                if (numbers[i + 2].first < 0)
                {
                    c = vertices[vertices.size() + numbers[i + 2].first];
                }
                else
                {
                    c = vertices[numbers[i + 2].first - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }


                // NORMALS
                if (numbers[0].second < 0)
                {
                    normalA = vertexNormals[vertexNormals.size() + numbers[0].second];
                }
                else
                {
                    normalA = vertexNormals[numbers[0].second - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                if (numbers[i + 1].second < 0)
                {
                    normalB = vertexNormals[vertexNormals.size() + numbers[i + 1].second];
                }
                else
                {
                    normalB = vertexNormals[numbers[i + 1].second - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                if (numbers[i + 2].second < 0)
                {
                    normalC = vertexNormals[vertexNormals.size() + numbers[i + 2].second];
                }
                else
                {
                    normalC = vertexNormals[numbers[i + 2].second - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                trianglesInFace[i] = { a, b, c, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, normalA, normalB, normalC };
            }
        }
        else  // Everything is present
        {
            std::vector<std::array<int, 3>> numbers;  // vertices and normals
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
                if (numbers[0][0] < 0)
                {
                    a = vertices[vertices.size() + numbers[0][0]];
                }
                else
                {
                    a = vertices[numbers[0][0] - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                if (numbers[i + 1][0] < 0)
                {
                    b = vertices[vertices.size() + numbers[i + 1][0]];
                }
                else
                {
                    b = vertices[numbers[i + 1][0] - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                if (numbers[i + 2][0] < 0)
                {
                    c = vertices[vertices.size() + numbers[i + 2][0]];
                }
                else
                {
                    c = vertices[numbers[i + 2][0] - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                // TEXTURES
                if (numbers[0][1] < 0)
                {
                    textureA = textureVertices[textureVertices.size() + numbers[0][1]];
                }
                else
                {
                    textureA = textureVertices[numbers[0][1] - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                if (numbers[i + 1][1] < 0)
                {
                    textureB = textureVertices[textureVertices.size() + numbers[i + 1][1]];
                }
                else
                {
                    textureB = textureVertices[numbers[i + 1][1] - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                if (numbers[i + 2][1] < 0)
                {
                    textureC = textureVertices[textureVertices.size() + numbers[i + 2][1]];
                }
                else
                {
                    textureC = textureVertices[numbers[i + 2][1] - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }


                // NORMALS
                if (numbers[0][2] < 0)
                {
                    normalA = vertexNormals[vertexNormals.size() + numbers[0][2]];
                }
                else
                {
                    normalA = vertexNormals[numbers[0][2] - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                if (numbers[i + 1][2] < 0)
                {
                    normalB = vertexNormals[vertexNormals.size() + numbers[i + 1][2]];
                }
                else
                {
                    normalB = vertexNormals[numbers[i + 1][2] - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                if (numbers[i + 2][2] < 0)
                {
                    normalC = vertexNormals[vertexNormals.size() + numbers[i + 2][2]];
                }
                else
                {
                    normalC = vertexNormals[numbers[i + 2][2] - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                trianglesInFace[i] = { a, b, c, textureA, textureB, textureC, normalA, normalB, normalC };
            }
        }
    }

    for (const Triangle& tr : trianglesInFace)
    {
        parentConverter->getTriangles().emplace_back(tr);
    }

    return true;
}

}  // namespace shaper