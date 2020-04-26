#pragma once

#include "I_InputParser.h"

#pragma warning(push)
#pragma warning(disable : 4201)
#include "glm.hpp"
#pragma warning(pop)

#include <filesystem>

namespace shaper {

class Converter;

class ObjParser : public I_InputParser
{
public:
    ObjParser(Converter* parentConverter, const std::filesystem::path& input);
    ~ObjParser() = default;

    ObjParser() = delete;
    ObjParser(const ObjParser& rhs) = delete;
    ObjParser(ObjParser&& rhs) = delete;

    ObjParser& operator=(const ObjParser& rhs) = delete;
    ObjParser& operator=(ObjParser&& rhs) = delete;

    virtual bool parse() override;

private:
    std::vector<glm::vec4> m_vertices;
    std::vector<glm::vec3> m_textureVertices;
    std::vector<glm::vec3> m_vertexNormals;

    bool parseVertexLine(const std::vector<std::string>& vec);
    bool parseTextureVertexLine(const std::vector<std::string>& vec);
    bool parseVertexNormalLine(const std::vector<std::string>& vec);
    bool parseFaceLine(const std::vector<std::string>& vec);
};
}  // namespace shaper