#include "TGAFile.h"

#include <fstream>

enum TGAHeader
{
	TGA_OFFSET_WIDTH = 12,
	TGA_OFFSET_HEIGHT = 14,
	TGA_OFFSET_PIXELS = 18
};

TGAFile::TGAFile(const std::string &filePath) :
	width(0),
	height(0),
	pixels(nullptr)
{
	std::ifstream file(filePath, std::ios::binary);

	if (!file.is_open())
		return;

	char header[18];
	file.read(header, 18);

	width = *reinterpret_cast<unsigned short*>(&header[TGA_OFFSET_WIDTH]);
	height = *reinterpret_cast<unsigned short*>(&header[TGA_OFFSET_HEIGHT]);
	unsigned int length = width * height;

	unsigned int* buffer = new unsigned int[length];
	pixels = new unsigned int[length];

	file.seekg(TGA_OFFSET_PIXELS, std::ios::beg);
	file.read(reinterpret_cast<char*>(&buffer[0]), length * sizeof(unsigned int));
	file.close();

	// TGA files store rows of pixels in reverse order (bottom to top)
	for (int srcRow = length - width, dstRow = 0; srcRow >= 0; srcRow -= width, dstRow += width)
	{
		std::memcpy(&pixels[dstRow], &buffer[srcRow], width * 4);
	}

	delete[] buffer;
}

TGAFile::~TGAFile()
{
	if (pixels)
		delete[] pixels;
}

unsigned int TGAFile::GetPixel(int offset)
{
	return pixels ? pixels[offset] : 0xFF00FFFF;
}