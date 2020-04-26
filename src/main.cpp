#include "Converter.h"
#include "Utilities.h"

#include <iostream>

using namespace shaper;

// questions:
// 1. winding order on .obj files? is there a default? in the provided link it doesnt say


int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    Converter conv(SupportedInputFormats::OBJ, utilities::getTestFilePath("nanosuit.obj"), SupportedOutputFormats::STL, utilities::getTestFilePath("test.stl"));

    //conv.addTranslation({ 100, 0, 100 });
    //conv.addRotation({ 1, 0, 0 }, 3.14f / 2);
    //conv.addTranslation({ 100, 0, 100 });
    //conv.addScaling({ 3, 3, 1 });


    if (!conv.convert())
    {
        std::cout << conv.getError() << "\n";
        return -1;
    }

    std::cout << "Mesh area: " << conv.calculateMeshArea() << "\n";
    std::cout << "Mesh volume: " << conv.calculateMeshVolume() << "\n";
    std::cout << "Is point inside: " << conv.isPointInsideTheMesh({ 0.35f, 12, 30.3 }) << "\n";

#ifdef _WIN32
    system("pause");  // TODO: windows only, remove this
#endif

    return 0;
}
