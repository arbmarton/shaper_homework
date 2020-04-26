#include "Utilities.h"

#include <sstream>
#include <iostream>
#include <fstream>

namespace shaper {
namespace utilities {

std::filesystem::path getTestFileFolderPath()
{
    std::string root = std::filesystem::current_path().parent_path().string();
    root.append("/testfiles/");
    return std::filesystem::path(root);
}

std::filesystem::path getTestFilePath(const std::string& fileName)
{
    return std::filesystem::path(getTestFileFolderPath().string() + fileName);
}

bool fileExists(const std::filesystem::path& path)
{
    return std::filesystem::exists(path);
}

std::vector<std::string> splitString(const std::string& toSplit, const char ch)
{
    std::stringstream stream(toSplit);
    std::string segment;
    std::vector<std::string> seglist;

    while (std::getline(stream, segment, ch))
    {
        seglist.push_back(segment);
    }

    return seglist;
}

//https://stackoverflow.com/a/51572325/6874134
std::vector<std::string> readTextFile(const std::filesystem::path& path)
{
    std::vector<std::string> ret;

    std::ifstream file(path.c_str());
    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            //std::cout << ret.emplace_back(line) << "\n";
            ret.emplace_back(line);
        }
        file.close();
    }

    return ret;
}

void printStringVector(const std::vector<std::string>& vec)
{
    for (const std::string& str : vec)
    {
        std::cout << str << "\n";
    }
}

}  // namespace utilities
}  // namespace shaper