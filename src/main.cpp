#include "Converter.h"
#include "Utilities.h"

#include <iostream>
#include <memory>
#include <chrono>

using namespace shaper;

// questions:
// 1. winding order on .obj files? is there a default? in the provided link it doesnt say


int main(int argc, char* argv[])
{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    std::unique_ptr<Converter> conv{ nullptr };

    if (argc != 5)
    {
        conv = std::make_unique<Converter>(
            SupportedInputFormats::OBJ, utilities::getTestFilePath("nanosuit.obj"), SupportedOutputFormats::STL, utilities::getTestFilePath("test.stl"));
    }
    else
    {
        const std::string inputFormat = argv[1];
        const std::string inputFile = argv[2];
        const std::string outputFormat = argv[3];
        const std::string outputFile = argv[4];

        SupportedInputFormats input;
        SupportedOutputFormats output;
        try
        {
            input = stringToInput(inputFormat);
            output = stringToOutput(outputFormat);
        } catch (const std::invalid_argument& exc)
        {
            std::cout << exc.what() << "\n";
            return -1;
        }

        if (!utilities::fileExists(utilities::getTestFilePath(inputFile + "." + inputFormat)))
        {
            std::cout << "The input file does not exist.\n";
            return -1;
        }

        conv = std::make_unique<Converter>(
            input, utilities::getTestFilePath(inputFile + "." + inputFormat), output, utilities::getTestFilePath(outputFile + "." + outputFormat));
    }

    //conv->addTranslation({ 100, 0, 100 });
    //conv->addRotation({ 1, 0, 0 }, 3.14f / 2);
    //conv->addTranslation({ 100, 0, 100 });
    //conv->addScaling({ 3, 3, 1 });


    if (!conv->convert())
    {
        std::cout << conv->getError() << "\n";
        return -1;
    }

    std::cout << "Mesh area: " << conv->calculateMeshArea() << "\n";
    std::cout << "Mesh volume: " << conv->calculateMeshVolume() << "\n";
    std::cout << "Is point inside: " << conv->isPointInsideTheMesh({ 0.35f, 12, 30.3 }) << "\n";

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;

#ifdef _WIN32
    system("pause");  // TODO: windows only, remove this
#endif

    return 0;
}
