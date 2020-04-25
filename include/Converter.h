#pragma once

#include "Enums.h"
#include "Triangle.h"

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
};

}  // namespace shaper