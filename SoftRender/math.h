#pragma once
#ifndef MATH_H
#define MATH_H
#include "SoftRenderer.h"

namespace Math {
	template <typename T>
	static T Lerp(const T& v1, const T& v2, float t)
	{
		return (v2 - v1) * t + v1;
	}

	template <typename T>
	static T Clamp(const T& v, const T& max, const T& min)
	{
		return v > max ? max : v < min ? min : v;
	}

	//转换到屏幕空间
	void transformScreen(point& v, int w, int h)
	{
		v.x = (v.x + 1.0f) * w * 0.5f;
		v.y = (1.0f - v.y) * h * 0.5f;
	}
	//透视除法
	void perspectiveDivide(vec4& v)
	{
		v.x /= v.w;
		v.y /= v.w;
		v.z /= v.w;
		v.w = 1.0f;
	}

};



#endif // !MATH_H

