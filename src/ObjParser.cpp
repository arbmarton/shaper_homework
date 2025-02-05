#include "ObjParser.h"
#include "Utilities.h"
#include "Converter.h"

#pragma warning(push)
#pragma warning(disable : 4201)
#include "glm.hpp"
#pragma warning(pop)

#include <iostream>
#include <array>
#include <algorithm>

namespace shaper {

ObjParser::ObjParser(Converter* parentConverter, const std::filesystem::path& input) noexcept
    : I_InputParser(parentConverter, input, SupportedInputFormats::OBJ)
{
}

bool ObjParser::parse()
{
    const std::vector<std::string> rawLineData{ utilities::readTextFile(m_inputFile) };

    if (rawLineData.size() == 0)
    {
        m_parentConverter->setError(m_inputFile.string() + " could not be opened or was empty.");
    }

    for (const std::string& str : rawLineData)
    {
        if (str.size() == 0)
        {
            continue;
        }

        std::vector<std::string> splitLine{ utilities::splitString(str, ' ') };

        // Remove empty strings
        splitLine.erase(std::remove(splitLine.begin(), splitLine.end(), ""), splitLine.end());

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
        else if (token[0] == '#')  // its a comment
        {
        }
        else
        {
            if (m_parentConverter->isLoggingEnabled())
            {
                std::cout << "PARSING: Ignoring token \"" << token << "\"\n";
            }
        }
    }

    return true;
}

bool ObjParser::parseVertexLine(const std::vector<std::string>& vec)
{
    if (vec.size() != 4 && vec.size() != 5)
    {
        m_parentConverter->setError("Vertex data parsing: incorrect line size");
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

    m_vertices.emplace_back(vertex);

    return true;
}

bool ObjParser::parseTextureVertexLine(const std::vector<std::string>& vec)
{
    if (vec.size() < 2 || vec.size() > 4)
    {
        m_parentConverter->setError("Vertex texture data parsing: incorrect line size");
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

    m_textureVertices.emplace_back(textureVertex);

    return true;
}

bool ObjParser::parseVertexNormalLine(const std::vector<std::string>& vec)
{
    if (vec.size() != 4)
    {
        m_parentConverter->setError("Vertex normals parsing: incorrect line size");
        return false;
    }

    glm::vec3 vertexNormal{ 0, 0, 0 };

    vertexNormal.x = std::stof(vec[1]);
    vertexNormal.y = std::stof(vec[2]);
    vertexNormal.z = std::stof(vec[3]);

    m_vertexNormals.emplace_back(vertexNormal);

    return true;
}


// https://stackoverflow.com/questions/23723993/converting-quadriladerals-in-an-obj-file-into-triangles
// Based on this the points should be regarded as triangle fans
bool ObjParser::parseFaceLine(const std::vector<std::string>& vec)
{
    if (vec.size() < 4)
    {
        m_parentConverter->setError("Face parsing: line is too short");
        return false;
    }

    // Check if the forward slashes are consistent
    const uint32_t referenceCount = utilities::countCharInString(vec[1], '/');
    for (size_t i = 2; i < vec.size(); ++i)
    {
        if (utilities::countCharInString(vec[i], '/') != referenceCount)
        {
            m_parentConverter->setError("Line has inconsistent slashes");
            return false;
        }
    }

    std::vector<Triangle> trianglesInFace;
    trianglesInFace.resize(vec.size() - 3);

    // Check if its a single number, the parsing is more simple then
    if (vec[1].find('/') == std::string::npos)
    {
        std::vector<size_t> numbers;
        numbers.reserve(vec.size() - 1);
        for (size_t i = 1; i < vec.size(); ++i)
        {
            numbers.push_back(std::stoul(vec[i]));
        }

        for (size_t i = 0; i < trianglesInFace.size(); ++i)
        {
            glm::vec3 a;
            glm::vec3 b;
            glm::vec3 c;

            if (numbers[0] < 0)
            {
                a = m_vertices[m_vertices.size() + numbers[0]];
            }
            else
            {
                a = m_vertices[numbers[0] - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
            }

            if (numbers[i + 1] < 0)
            {
                b = m_vertices[m_vertices.size() + numbers[i + 1]];
            }
            else
            {
                b = m_vertices[numbers[i + 1] - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
            }

            if (numbers[i + 2] < 0)
            {
                c = m_vertices[m_vertices.size() + numbers[i + 2]];
            }
            else
            {
                c = m_vertices[numbers[i + 2] - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
            }

            trianglesInFace[i] = { a, b, c, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };
        }
    }
    else
    {
        // Texture coords are missing
        if (utilities::splitString(vec[1], '/')[1] == "")
        {
            std::vector<std::pair<size_t, size_t>> numbers;  // vertices and normals
            numbers.reserve(vec.size() - 1);
            for (size_t i = 1; i < vec.size(); ++i)
            {
                const std::vector<std::string> splitGroup{ utilities::splitString(vec[i], '/') };
                numbers.push_back(std::make_pair<size_t, size_t>(std::stoul(splitGroup[0]), std::stoul(splitGroup[2])));
            }

            for (size_t i = 0; i < trianglesInFace.size(); ++i)
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
                    a = m_vertices[m_vertices.size() + numbers[0].first];
                }
                else
                {
                    a = m_vertices[numbers[0].first - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                if (numbers[i + 1].first < 0)
                {
                    b = m_vertices[m_vertices.size() + numbers[i + 1].first];
                }
                else
                {
                    b = m_vertices[numbers[i + 1].first - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                if (numbers[i + 2].first < 0)
                {
                    c = m_vertices[m_vertices.size() + numbers[i + 2].first];
                }
                else
                {
                    c = m_vertices[numbers[i + 2].first - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }


                // NORMALS
                if (numbers[0].second < 0)
                {
                    normalA = m_vertexNormals[m_vertexNormals.size() + numbers[0].second];
                }
                else
                {
                    normalA = m_vertexNormals[numbers[0].second - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                if (numbers[i + 1].second < 0)
                {
                    normalB = m_vertexNormals[m_vertexNormals.size() + numbers[i + 1].second];
                }
                else
                {
                    normalB = m_vertexNormals[numbers[i + 1].second - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                if (numbers[i + 2].second < 0)
                {
                    normalC = m_vertexNormals[m_vertexNormals.size() + numbers[i + 2].second];
                }
                else
                {
                    normalC = m_vertexNormals[numbers[i + 2].second - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                trianglesInFace[i] = { a, b, c, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, normalA, normalB, normalC };
            }
        }
        else  // Everything is present
        {
            std::vector<std::array<size_t, 3>> numbers;  // vertices and normals
            numbers.reserve(vec.size() - 1);
            for (size_t i = 1; i < vec.size(); ++i)
            {
                const std::vector<std::string> splitGroup{ utilities::splitString(vec[i], '/') };
                numbers.push_back({ std::stoul(splitGroup[0]), std::stoul(splitGroup[1]), std::stoul(splitGroup[2]) });
            }

            for (size_t i = 0; i < trianglesInFace.size(); ++i)
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
                    a = m_vertices[m_vertices.size() + numbers[0][0]];
                }
                else
                {
                    a = m_vertices[numbers[0][0] - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                if (numbers[i + 1][0] < 0)
                {
                    b = m_vertices[m_vertices.size() + numbers[i + 1][0]];
                }
                else
                {
                    b = m_vertices[numbers[i + 1][0] - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                if (numbers[i + 2][0] < 0)
                {
                    c = m_vertices[m_vertices.size() + numbers[i + 2][0]];
                }
                else
                {
                    c = m_vertices[numbers[i + 2][0] - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                // TEXTURES
                if (numbers[0][1] < 0)
                {
                    textureA = m_textureVertices[m_textureVertices.size() + numbers[0][1]];
                }
                else
                {
                    textureA = m_textureVertices[numbers[0][1] - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                if (numbers[i + 1][1] < 0)
                {
                    textureB = m_textureVertices[m_textureVertices.size() + numbers[i + 1][1]];
                }
                else
                {
                    textureB = m_textureVertices[numbers[i + 1][1] - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                if (numbers[i + 2][1] < 0)
                {
                    textureC = m_textureVertices[m_textureVertices.size() + numbers[i + 2][1]];
                }
                else
                {
                    textureC = m_textureVertices[numbers[i + 2][1] - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }


                // NORMALS
                if (numbers[0][2] < 0)
                {
                    normalA = m_vertexNormals[m_vertexNormals.size() + numbers[0][2]];
                }
                else
                {
                    normalA = m_vertexNormals[numbers[0][2] - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                if (numbers[i + 1][2] < 0)
                {
                    normalB = m_vertexNormals[m_vertexNormals.size() + numbers[i + 1][2]];
                }
                else
                {
                    normalB = m_vertexNormals[numbers[i + 1][2] - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                if (numbers[i + 2][2] < 0)
                {
                    normalC = m_vertexNormals[m_vertexNormals.size() + numbers[i + 2][2]];
                }
                else
                {
                    normalC = m_vertexNormals[numbers[i + 2][2] - 1];  // -1 because of the offset in the .obj specification, the counting starts from 1
                }

                trianglesInFace[i] = { a, b, c, textureA, textureB, textureC, normalA, normalB, normalC };
            }
        }
    }

    for (const Triangle& tr : trianglesInFace)
    {
        m_parentConverter->getTriangles().emplace_back(tr);
    }

    return true;
}

}  // namespace shaper