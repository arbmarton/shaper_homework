#include "Converter.h"
#include "Utilities.h"

using namespace shaper;

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    Converter conv(SupportedInputFormats::OBJ, utilities::getTestFilePath("pyramid.obj"), SupportedOutputFormats::STL, utilities::getTestFilePath("test.stl"));

    if (!conv.convert())
    {
        return -1;
    }

#ifdef _WIN32
    system("pause");  // TODO: windows only, remove this
#endif

    return 0;
}
