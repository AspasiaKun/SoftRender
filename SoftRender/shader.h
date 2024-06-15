#pragma once
#ifndef SHADER_H
#define SHADER_H
#include "matrix.h"
#include "ConstUtil.h"



class Shader {
public:
	Shader() {}

	//lambert π‚’’
	float shade(const point& coord, vec4& normal, vec4& light_pos, vec4& camera_pos);

};





#endif // !SHADER_H
