#include "Converter.h"
#include "Utilities.h"

#include <iostream>


#pragma warning(push)
#pragma warning(disable : 4201)
#include "glm.hpp"
#pragma warning(pop)


namespace shaper {

}

using namespace shaper;

int
is_big_endian_machine(void)
{
    static
        unsigned
        tst = 1;

    static
        int
        stat = (*(unsigned char*)&tst) != 1;

    return stat;
}

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    auto aaaa = is_big_endian_machine();
    (void)aaaa;

    glm::vec3 sdfdsf;
    aaaa = sizeof(sdfdsf);


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
