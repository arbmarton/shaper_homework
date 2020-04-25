#pragma once

#include "I_InputParser.h"

#include <filesystem>

namespace shaper {

class Converter;

class ObjParser : public I_InputParser
{
public:
    ObjParser(Converter* parentConverter, const std::filesystem::path& input);

    virtual void parse() override;

private:
};
}  // namespace shaper