#pragma once
#ifndef VEC4_H
#define VEC4_H
#include<cmath>

class vec4 {
public:
	float x, y, z, w;

	vec4(float _x, float _y, float _z, float _w){x = _x; y = _y; z = _z; w = _w;}

	vec4(float _x, float _y, float _z){x = _x; y = _y; z = _z; w = 1.0f;}

	vec4(const vec4& v){*this = v;}

	vec4() { x = y = z = 0.0; w = 1.0; }

	vec4 operator +(const vec4& v) const
	{
		return { x + v.x, y + v.y, z + v.z };
	}

	vec4 operator -(const vec4& v)const
	{
		return { x - v.x,y - v.y,z - v.z };
	}

	vec4& operator +=(const vec4& v)
	{ 
		return { *this = *this + v };
	}

	vec4& operator -=(const vec4& v)
	{
		return { *this = *this + v };
	}

	vec4 operator *(const float v)const
	{
		return { x * v,y * v,z * v };
	}



	vec4 operator *(const vec4& v)const
	{
		return{ x * v.x,y * v.y,z * v.z };
	}

	vec4 operator /(const float v)const
	{
		return { x / v,y / v,z / v };
	}

	float dot(const vec4& v)const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	vec4 cross(const vec4& v)const
	{
		return { y * v.z - z * v.y,
				z * v.x - x * v.z,
				x * v.y - y * v.x };
	}

	float LengthSqrt() const
	{
		return dot(*this);
	}

	float Length() const
	{
		return std::sqrt(LengthSqrt());
	}

	vec4 normalize() const
	{
		vec4 tmp;
		auto len = Length();
		if (len != 0)
		{
			auto v = 1.0f / len;
			tmp.x = x * v;
			tmp.y = y * v;
			tmp.z = z * v;
		}
		return tmp;
	}


};

 inline vec4 operator *(const float u, vec4 v)
{
	return v * u;
}
using point = vec4;
using Color = vec4;


inline vec4 operator -(const vec4 v)
{
	return { -v.x,-v.y,-v.z };
}

#endif // !VEC4_H
