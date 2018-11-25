#pragma once

#include <string>

// Encapsulates a TGA bitmap file
class TGAFile
{
public:
	TGAFile(const std::string &filePath);
	~TGAFile();

	unsigned short Width() const { return width; }
	unsigned short Height() const { return height; }
	unsigned int* Pixels() const { return pixels; }

	unsigned int GetPixel(int offset) const { return pixels ? pixels[offset] : errorColour; }

private:
	unsigned short width;
	unsigned short height;
	unsigned int* pixels;
	unsigned int errorColour;
};