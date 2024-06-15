#pragma once
#ifndef LIGHT_H
#define LIGHT_H
#include "ConstUtil.h"
#include "matrix.h"

class Light {
public:
	Light(vec4 pos = { 0.0f,0.0f,0.0f,0.0f }) :_pos(pos) {}

	point _pos;
	vec4 _dir;

};

#endif // !LIGHT_H

