#pragma once

#ifndef TGAIMAGE_H
#define TGAIMAGE_H

#include <cstdint>
#include <fstream>
#include <vector>

#pragma pack(push,1) //按照1字节来对齐

struct TGAHeader {
	std::uint8_t idlength = 0;
	std::uint8_t colormaptype = 0;
	std::uint8_t datatypecode = 0;
	std::uint8_t colormaporigin = 0;
	std::uint8_t colormaplength = 0;
	std::uint8_t colormapsize = 0;
	std::uint8_t x_origin = 0;
	std::uint8_t y_origin = 0;
	std::uint8_t width = 0;
	std::uint8_t height = 0;
	std::uint8_t bitsperpixel = 0;
	std::uint8_t imagedescriptor = 0;
};
#pragma pack(pop)

struct TGAColor {
	TGAColor() {};
	TGAColor(const int& v1, const int& v2, const int& v3, const int& v4, std::uint8_t bpp) {
		bgra[0] = v1;
		bgra[1] = v2;
		bgra[2] = v3;
		bgra[3] = v4;
		bytespp = bpp;
	};
	std::uint8_t bgra[4] = { 0,0,0,0 };
	std::uint8_t bytespp = 4;
	std::uint8_t& operator[](const int i) { return bgra[i]; }
};

struct TGAImage {
	enum Format { GRAYSCALE = 1, RGB = 3, RGBA = 4};

	TGAImage() = default;
	TGAImage(const int w, const int h, const int bpp);
	bool read_tga_file(const std::string filename);
	bool write_tga_file(const std::string filename, const bool vflip = true, const bool rle = true) const;
	void flip_horizontally();
	void flip_vertically();
	TGAColor get(const int x, const int y) const;
	void set(const int x, const int y, const TGAColor& c);
	int get_width() const;
	int get_height() const;
private:
	bool load_rle_data(std::ifstream& in);
	bool unload_rle_data(std::ofstream& out) const;

	int w = 0;
	int h = 0;
	std::uint8_t bpp = 0;  //bits per pixel
	std::vector<std::uint8_t> data = {};

};


#endif // !TGAIMAGE_H