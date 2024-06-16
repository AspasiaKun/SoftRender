#include <iostream>
#include <sstream>
#include "model.h"


Model::Model(const std::string filename) {
	std::ifstream in;
	in.open(filename, std::ifstream::in);
	if (in.fail()) return;
	std::string line;
	while (!in.eof()) {
		std::getline(in, line);
		std::istringstream iss(line.c_str());
		char trash;
		if (!line.compare(0, 2, "v ")) {
			iss >> trash;
			vec4 v = vec4();
			for (int i = 0; i < 3; i++) {
				float tmp;
				iss >> tmp;
				v[i] = tmp;
			}
			pos_coord.push_back(v);
		}
		else if (!line.compare(0, 3, "vn ")) {
			iss >> trash >> trash;
			vec4 n = vec4();
			for (int i = 0; i < 3; i++) {
				float tmp;
				iss >> tmp;
				n[i] = tmp;
			}
			norms.push_back(n.normalize());
		}
		else if (!line.compare(0, 3, "vt ")) {
			iss >> trash >> trash;
			vec4 uv = vec4();
			for (int i = 0; i < 2; i++) {
				float tmp;
				iss >> tmp;
				uv[i] = tmp;
			}
			tex_coord.push_back(vec4(uv.x,1-uv.y));
		}
		else if (!line.compare(0, 2, "f ")) {
			int f, t, n;
			iss >> trash;
			int cnt = 0;
			while (iss >> f>>trash>>t>>trash>>n)
			{
				facet_vrt.push_back(--f);
				facet_tex.push_back(--t);
				facet_nrm.push_back(--n);
				cnt++;
			}
			if (3!=cnt)
			{
				std::cerr << "Error: the obj file is supposed to be triangulated" << std::endl;
				return;
			}
		}
	}
	std::cerr << "# v# " << nverts() << " f# " << nfaces() << " vt# " << tex_coord.size() << " vn# " << norms.size() << std::endl;
	load_texture(filename, "_diffuse.tga", diffusemap);
	load_texture(filename, "_nm_tangent.tga", normalmap);
	load_texture(filename, "_spec.tga", specularmap);
}

int Model::nverts() const
{
	return 0;
}

int Model::nfaces() const
{
	return 0;
}

vec4 Model::normal(const int iface, const int nthvert) const
{
	return vec4();
}

vec4 Model::normal(const vec4& uv) const
{
	return vec4();
}

vec4 Model::vert(const int i) const
{
	return vec4();
}

vec4 Model::vert(const int iface, const int nthvert) const
{
	return vec4();
}

vec4 Model::uv(const int iface, const int nthvert) const
{
	return vec4();
}

void Model::load_texture(const std::string filename, const std::string suffix, TGAImage& img)
{
}