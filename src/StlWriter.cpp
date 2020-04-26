#include "StlWriter.h"
#include "Converter.h"
#include "Triangle.h"

#include <fstream>

namespace shaper {

StlWriter::StlWriter(Converter* parentConverter, const std::filesystem::path& outFile) noexcept
    : I_OutWriter(parentConverter, outFile, SupportedOutputFormats::STL)
{
}

bool StlWriter::write()
{
    char dummyheader[80] = { '0' };

    std::ofstream outfile;

    const int32_t triangles = int32_t(m_parentConverter->getTriangles().size());

    outfile.open(m_filePath.string(), std::ios::out | std::ios::binary);

    if (!outfile.is_open())
    {
        m_parentConverter->setError("Outfile could not be opened.");
        return false;
    }

    outfile.write(dummyheader, sizeof(dummyheader));
    outfile.write((char*)(&triangles), sizeof(int32_t));

    for (const Triangle& tr : m_parentConverter->getTriangles())
    {
        // Normal
        const glm::vec3 surfaceNormal = tr.getSurfaceNormal();
        outfile.write((char*)&surfaceNormal.x, sizeof(float));
        outfile.write((char*)&surfaceNormal.y, sizeof(float));
        outfile.write((char*)&surfaceNormal.z, sizeof(float));

        // Vertex a
        outfile.write((char*)&tr.m_a.x, sizeof(float));
        outfile.write((char*)&tr.m_a.y, sizeof(float));
        outfile.write((char*)&tr.m_a.z, sizeof(float));

        // Vertex b
        outfile.write((char*)&tr.m_b.x, sizeof(float));
        outfile.write((char*)&tr.m_b.y, sizeof(float));
        outfile.write((char*)&tr.m_b.z, sizeof(float));

        // Vertex c
        outfile.write((char*)&tr.m_c.x, sizeof(float));
        outfile.write((char*)&tr.m_c.y, sizeof(float));
        outfile.write((char*)&tr.m_c.z, sizeof(float));

        // Attribute thing
        outfile.write("0", 1);
        outfile.write("0", 1);
    }

    outfile.close();

    return true;
}

}  // namespace shaper