#include "Converter.h"
#include "Utilities.h"

#include <iostream>

namespace shaper {

}

using namespace shaper;

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    //glm::vec3 fsdfsd(1, 1, 1);

    //auto a = shaper::utilities::readTextFile("d:/Dev/Misc/shaper_homework/src/main.cpp");

    //shaper::utilities::printStringVector(a);

    auto a = utilities::splitString("4/5/6", '/');
    auto b = utilities::splitString("4//6", '/');
    auto c = utilities::splitString("4", '/');


    Converter conv(SupportedInputFormats::OBJ, utilities::getTestFilePath("nanosuit.obj"), SupportedOutputFormats::STL, utilities::getTestFilePath("test.stl"));

    conv.convert();

    //std::cout << "Hello world!\n";

    system("pause");  // TODO: windows only, remove this

    return 0;
}
