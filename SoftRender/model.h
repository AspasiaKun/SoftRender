#pragma once
#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include "vertex.h"
#include "tgaimage.h"

class Model {
	std::vector<vertex> verts{};

	std::vector<vec4> pos_coord{}; // array of vertices
	std::vector<vec4> tex_coord{}; // per-vertex array of tex coords
	std::vector<vec4> norms{};     // per-vertex array of normal vectors

	std::vector<int> facet_vrt{};// indices in the above arrays
	std::vector<int> facet_tex{};
	std::vector<int> facet_nrm{};

	TGAImage diffusemap{};
	TGAImage normalmap{};
	TGAImage specularmap{};

	void load_texture(const std::string filename, const std::string suffix, TGAImage& img);

public:
	Model(const std::string finename);
	int nverts() const;
	int nfaces() const;
	vec4 normal(const int iface, const int nthvert) const;
	vec4 normal(const vec4& uv) const;
	vec4 vert(const int i) const;
	vec4 vert(const int iface, const int nthvert) const;
	vec4 uv(const int iface, const int nthvert) const;
	const TGAImage diffuse() const { return diffusemap; }
	const TGAImage specular() const { return specularmap; }
};









#endif // !MODEL_H
