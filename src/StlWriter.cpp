#include "StlWriter.h"
#include "Converter.h"
#include "Triangle.h"

#include <fstream>

namespace shaper {

StlWriter::StlWriter(Converter* parentConverter, const std::filesystem::path& outFile)
	: I_OutWriter(parentConverter, outFile, SupportedOutputFormats::STL)
{

}

void StlWriter::write()
{
	char dummyheader[80];

	std::ofstream outfile;

	const int32_t triangles = int32_t(parentConverter->getTriangles().size());

	outfile.open(filePath.string(), std::ios::out | std::ios::binary);
	outfile.write(dummyheader, sizeof(dummyheader));
	outfile.write((char*)(&triangles), sizeof(int32_t));

	for (const Triangle& tr : parentConverter->getTriangles())
	{
		// Normal
		const glm::vec3 surfaceNormal = tr.getSurfaceNormal();
		outfile.write((char*)&surfaceNormal.x, sizeof(float));
		outfile.write((char*)&surfaceNormal.y, sizeof(float));
		outfile.write((char*)&surfaceNormal.z, sizeof(float));

		// Vertex a
		outfile.write((char*)&tr.a.x, sizeof(float));
		outfile.write((char*)&tr.a.y, sizeof(float));
		outfile.write((char*)&tr.a.z, sizeof(float));

		// Vertex b
		outfile.write((char*)&tr.b.x, sizeof(float));
		outfile.write((char*)&tr.b.y, sizeof(float));
		outfile.write((char*)&tr.b.z, sizeof(float));

		// Vertex c
		outfile.write((char*)&tr.c.x, sizeof(float));
		outfile.write((char*)&tr.c.y, sizeof(float));
		outfile.write((char*)&tr.c.z, sizeof(float));

		// Attribute thing
		outfile.write("0", 1);
		outfile.write("0", 1);
	}

	outfile.close();
}

}