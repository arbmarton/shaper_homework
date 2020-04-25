#pragma once

#include "Enums.h"
//#include "I_InputParser.h"
//#include "I_OutWriter.h"

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
    std::optional<std::string> convert() const;

private:
    std::string error;

    std::unique_ptr<I_InputParser> parser;
    std::unique_ptr<I_OutWriter> writer;

    // DATA members like points and faces and stuff here
};

}  // namespace shaper