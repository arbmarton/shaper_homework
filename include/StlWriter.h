#pragma once

#include "I_OutWriter.h"

namespace shaper {
class StlWriter : public I_OutWriter
{
public:
	StlWriter(Converter* parentConverter, const std::filesystem::path& outFile);

	virtual void write() override;

private:

};
}  // namespace shaper