# shaper_homework

# Purpose
3D file format converter. Currently supports .OBJ to binary .STL conversion.

# Usage
Either from command line, or by editing the source code. If used from the command line, there must be four arguments present:
- input format
- input filename (without extension)
- output format
- output filename (without extension)

If the compiled software is ran outside of the development environment the files need to placed in the following folder relative from the .exe: ${exepath}/../testfiles/
The software should give you error messages to the console should something go awry. (But no guarantees :))
  
# How to extend
You need to add your file format to the SupportedInputFormats and SupportedOutputFormats enums, inherit from the I_OutWriter and the I_InputParser interfaces, and override the write() and parse() functions. The error messages should guide you through the rest.
