#include "Converter.h"
#include "Utilities.h"

using namespace shaper;

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    Converter conv(SupportedInputFormats::OBJ, utilities::getTestFilePath("nanosuit.obj"), SupportedOutputFormats::STL, utilities::getTestFilePath("test.stl"));

    conv.addTranslation({ 100, 0, 100 });
    conv.addRotation({ 1, 0, 0 }, 3.14f / 2);
    conv.addTranslation({ 100, 0, 100 });
    conv.addScaling({ 3, 3, 1 });


    if (!conv.convert())
    {
        return -1;
    }

#ifdef _WIN32
    system("pause");  // TODO: windows only, remove this
#endif

    return 0;
}
