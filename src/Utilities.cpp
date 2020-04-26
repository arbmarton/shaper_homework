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

// http://www.cplusplus.com/forum/beginner/114790/
std::vector<std::string> splitString(const std::string& toSplit, const char ch)
{
    size_t start = 0;
    size_t end = toSplit.find_first_of(ch);

    std::vector<std::string> output;

    while (end <= std::string::npos)
    {
        output.emplace_back(toSplit.substr(start, end - start));

        if (end == std::string::npos)
        {
            break;
        }

        start = end + 1;
        end = toSplit.find_first_of(ch, start);
    }

    return output;
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
            ret.emplace_back(line);
        }
        file.close();
    }

    return ret;
}

uint32_t countCharInString(const std::string& str, const char ch)
{
    uint32_t ret{ 0 };

    for (const char c : str)
    {
        if (c == ch)
        {
            ret++;
        }
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