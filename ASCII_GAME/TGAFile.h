#pragma once

#include <string>

class TGAFile
{
public:
	TGAFile(const std::string &filePath);
	~TGAFile();

	unsigned short Width() const { return width; }
	unsigned short Height() const { return height; }
	unsigned int* Pixels() const { return pixels; }

	unsigned int GetPixel(int offset);

private:
	unsigned short width;
	unsigned short height;
	unsigned int* pixels;
};