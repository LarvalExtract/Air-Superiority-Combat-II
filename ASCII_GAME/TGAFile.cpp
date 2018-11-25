#include "TGAFile.h"

#include <fstream>

// http://www.paulbourke.net/dataformats/tga/
enum TGAHeader
{
	TGA_OFFSET_WIDTH = 12,
	TGA_OFFSET_HEIGHT = 14,
	TGA_OFFSET_BITDEPTH = 16,
	TGA_OFFSET_PIXELS = 18
};

TGAFile::TGAFile(const std::string &filePath) :
	width(0),
	height(0),
	pixels(nullptr)
{
	std::ifstream file(filePath, std::ios::binary);

	// Error: Couldn't open TGA file
	if (!file.is_open())
	{
		errorColour = 0x0000FFFF;	// Set pixel to red
		return;
	}

	char header[18];
	file.read(header, 18);

	width = *reinterpret_cast<unsigned short*>(&header[TGA_OFFSET_WIDTH]);
	height = *reinterpret_cast<unsigned short*>(&header[TGA_OFFSET_HEIGHT]);

	// Error: TGA file is not 32 bits
	if (header[TGA_OFFSET_BITDEPTH] != 32)
	{
		errorColour = 0xFF00FFFF;	// Set pixel to pink
		return;
	}

	unsigned int length = width * height;

	pixels = new unsigned int[length];
	unsigned int* buffer = new unsigned int[length];

	// Copy pixels to temporary buffer
	file.seekg(TGA_OFFSET_PIXELS, std::ios::beg);
	file.read(reinterpret_cast<char*>(&buffer[0]), length * sizeof(unsigned int));
	file.close();

	// TGA files store rows of pixels from bottom to top
	// Copy rows of pixels from *buffer to *pixels in reverse order
	int srcRow = length - width;
	for (int dstRow = 0; srcRow >= 0; dstRow += width)
	{
		std::memcpy(&pixels[dstRow], &buffer[srcRow], width * 4);
		srcRow -= width;
	}

	delete[] buffer;
}

TGAFile::~TGAFile()
{
	if (pixels)
		delete[] pixels;
}