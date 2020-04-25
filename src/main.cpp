#include "Converter.h"
#include "Utilities.h"

#include <iostream>

#include "glm.hpp"

namespace shaper {

}

using namespace shaper;

int main(int argc, char** argv)
{
    glm::vec3 fsdfsd(1, 1, 1);

    auto a = shaper::utilities::readTextFile("d:/Dev/Misc/shaper_homework/src/main.cpp");

    shaper::utilities::printStringVector(a);


    Converter conv(SupportedInputFormats::OBJ, utilities::getTestFilePath("box.obj"), SupportedOutputFormats::STL, utilities::getTestFilePath("test.stl"));

    std::cout << "Hello world!\n";

    system("pause");  // TODO: windows only, remove this

    return 0;
}
