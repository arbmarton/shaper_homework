#pragma once

#include "I_InputParser.h"

#include <filesystem>

namespace shaper {

class Converter;

class ObjParser : public I_InputParser
{
public:
    ObjParser(Converter* parentConverter, const std::filesystem::path& input);

    virtual std::optional<std::string> parse() override;

private:
    std::optional<std::string> parseVertexLine(const std::vector<std::string>& vec);
    std::optional<std::string> parseTextureVertexLine(const std::vector<std::string>& vec);
    std::optional<std::string> parseVertexNormalLine(const std::vector<std::string>& vec);
};
}  // namespace shaper