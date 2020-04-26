#pragma once

#include <vector>
#include <string>
#include <filesystem>

namespace shaper {
namespace utilities {

std::filesystem::path getTestFileFolderPath();
std::filesystem::path getTestFilePath(const std::string& fileName);
bool fileExists(const std::filesystem::path& path);

std::vector<std::string> splitString(const std::string& toSplit, const char ch);
std::vector<std::string> readTextFile(const std::filesystem::path& path);
uint32_t countCharInString(const std::string& str, const char ch);

void printStringVector(const std::vector<std::string>& vec);

}  // namespace utilities
}  // namespace shaper