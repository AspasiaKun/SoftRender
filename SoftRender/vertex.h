#pragma once
#ifndef VERTEX_H
#define VERTEX_H

#include "vec4.h"
#include <iostream>

class vertex{
public:
	point coord;
	Color color;
	vec4 normal; //vec3
	vec4 tex_coord; // vec2
};





#endif // !VERTEX_H

