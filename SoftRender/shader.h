#pragma once
#ifndef SHADER_H
#define SHADER_H
#include "matrix.h"
#include "SoftRenderer.h"



class Shader {
public:
	Shader() {}

	//lambert π‚’’
	float shade(const point& coord, vec4& normal,vec4& light_pos,vec4& camera_pos )
	{
		vec4 lightDir = (light_pos- coord).normalize();
		vec4 viewDir = (camera_pos - coord).normalize();
		float light_intensity = 10.0f;
		float dis = (light_pos - coord).Length();

		//ambient
		float ambient = 0.1f;

		//diffuse

		float diffuse = light_intensity / dis * max(0.0f,lightDir.dot(normal.normalize()));

		//specular
		vec4 h = (lightDir + viewDir).normalize();

		float specular = 5.0f * std::pow(max(h.dot(normal.normalize()),0.0f), 200);

		return (ambient +diffuse+specular);

		

	}

};





#endif // !SHADER_H
