#include "tgaimage.h"

#include <iostream>
#include <cstring>

TGAImage::TGAImage(const int w, const int h, const int bpp) 
	: w(w), h(h), bpp(bpp), data(w* h* bpp, 0) {}

bool TGAImage::read_tga_file(const std::string filename) {
	std::ifstream in;
	in.open(filename, std::ios::binary);
	if (!in.is_open()) {
		std::cerr << "can't open file" << filename << std::endl;
		return false;
	}
	TGAHeader header;
	in.read(reinterpret_cast<char*>(&header), sizeof(header));
	if (!in.good()) {
		std::cerr << "an error occured while reading the header" << std::endl;
		return false;
	}
	w = header.width;
	h = header.height;
	bpp = header.bitsperpixel;
	if (w <= 0 || h <= 0 || (bpp != GRAYSCALE && bpp != RGB && bpp != RGBA)) {
		std::cerr << "bad bpp (or width/height) value" << std::endl;
		return false;
	}
	size_t nbytes = bpp * w * h;
	data = std::vector<std::uint8_t>(nbytes, 0);
	if (header.datatypecode == 3 || header.datatypecode == 2) {
		in.read(reinterpret_cast<char*>(data.data()), nbytes);
		if (!in.good()) {
			std::cerr << "an error occured while reading the data" << std::endl;
			return false;
		}
	}
	else if (header.datatypecode == 10 || header.datatypecode == 11) {
		if (!load_rle_data(in)) {
			std::cerr << "an error occured while reading the data" << std::endl;
			return false;
		}
	}
	else {
		std::cerr << "unknow file format " << (int)header.datatypecode << std::endl;
		return false;
	}
	if (!(header.imagedescriptor & 0x20))
		flip_vertically();
	if (header.imagedescriptor & 0x10)
		flip_horizontally();
	std::cerr << w << "x" << h << "/" << bpp * 8 << std::endl;
	return true;
}

bool TGAImage::write_tga_file(const std::string filename, const bool vflip, const bool rle) const
{
	return false;
}

void TGAImage::flip_horizontally()
{
}

void TGAImage::flip_vertically()
{
}

TGAColor TGAImage::get(const int x, const int y) const
{
	return TGAColor();
}

void TGAImage::set(const int x, const int y, const TGAColor& c)
{
}

int TGAImage::get_width() const
{
	return 0;
}

int TGAImage::get_height() const
{
	return 0;
}

bool TGAImage::load_rle_data(std::ifstream& in)
{
	return false;
}

bool TGAImage::unload_rle_data(std::ofstream& out) const
{
	return false;
}

