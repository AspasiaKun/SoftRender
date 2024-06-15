#pragma once
#ifndef MATRIX_H
#define MATRIX_H

#include "vec4.h"
#include <cmath>
#include <algorithm>
#include <cstring>

class matrix4f {
public:
	float m[4][4];

	matrix4f() {  }

	matrix4f(float a1, float a2, float a3, float a4,
		float b1, float b2, float b3, float b4,
		float c1, float c2, float c3, float c4,
		float d1,float d2,float d3,float d4)
	{
		m[0][0] = a1; m[0][1] = a2; m[0][2] = a3; m[0][3] = a4;
		m[1][0] = b1; m[1][1] = b2; m[1][2] = b3; m[1][3] = b4;
		m[2][0] = c1; m[2][1] = c2; m[2][2] = c3; m[2][3] = c4;
		m[3][0] = d1; m[3][1] = d2; m[3][2] = d3; m[3][3] = d4;
	}

	void Zero() { std::memset(&m, 0, sizeof(m)); }


	matrix4f operator*(const matrix4f& v)const
	{
		matrix4f mat;
		for (auto x = 0; x < 4; x++)
		{
			for (auto y = 0; y < 4; y++)
			{
				mat.m[x][y] = m[x][0] * v.m[0][y] +
					m[x][1] * v.m[1][y] +
					m[x][2] * v.m[2][y] +
					m[x][3] * v.m[3][y];
			}
		}
		return mat;
	}

	matrix4f& Indentity()
	{
		Zero();
		m[0][0] = 1.0f;
		m[1][1] = 1.0f;
		m[2][2] = 1.0f;
		m[3][3] = 1.0f;
		return *this;
	}

	//得到旋转矩阵，绕某一轴旋转就把该轴置为1，其余为0.
	matrix4f& Rotate(float x, float y, float z, float radian)
	{
		float cos0 = std::cos(radian);
		float sin0 = std::sin(radian);
		matrix4f mat;
		mat.Indentity();
		mat.m[0][0] = x * x * (1 - cos0) + cos0;
		mat.m[0][1] = x * y * (1 - cos0) + z * sin0;
		mat.m[0][2] = x * y * (1 - cos0) - y * sin0;

		mat.m[1][0] = x * y * (1 - cos0) - z * sin0;
		mat.m[1][1] = y * y * (1 - cos0) + cos0;
		mat.m[1][2] = y * z * (1 - cos0) + x * sin0;

		mat.m[2][0] = x * z * (1 - cos0) + y * sin0;
		mat.m[2][1] = y * z * (1 - cos0) - x * sin0;
		mat.m[2][2] = z * z * (1 - cos0) + cos0;
		return mat;
	}

	matrix4f& Translate(float x, float y, float z)
	{
		matrix4f mat;
		mat.Indentity();
		mat.m[3][0] = x;
		mat.m[3][1] = y;
		mat.m[3][2] = z;
		return (*this = this->operator*(mat));
	}

	matrix4f& Scale(float x, float y, float z)
	{
		matrix4f mat;
		mat.Indentity();
		mat.m[0][0] = x;
		mat.m[1][1] = y;
		mat.m[2][2] = z;
		return (*this = this->operator*(mat));
	}

	void transpose() {
		std::swap(m[0][1], m[1][0]);
		std::swap(m[0][2], m[2][0]);
		std::swap(m[0][3], m[3][0]);
		std::swap(m[1][2], m[2][1]);
		std::swap(m[1][3], m[3][1]);
		std::swap(m[2][3], m[3][2]);
	}

	matrix4f getInverse() {
		double E_Matrix[4][4];
		double mik;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (i == j)
					E_Matrix[i][j] = 1.00;
				else
					E_Matrix[i][j] = 0.00;
			}
		}
		double CalcuMatrix[4][8];
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				CalcuMatrix[i][j] = this->m[i][j];
			}
			for (int k = 4; k < 8; k++)
			{
				CalcuMatrix[i][k] = E_Matrix[i][k - 4];
			}
		}

		for (int i = 1; i <= 4 - 1; i++)
		{
			for (int j = i + 1; j <= 4; j++)
			{
				mik = CalcuMatrix[j - 1][i - 1] / CalcuMatrix[i - 1][i - 1];
				for (int k = i + 1; k <= 8; k++)
				{
					CalcuMatrix[j - 1][k - 1] -= mik * CalcuMatrix[i - 1][k - 1];
				}
			}
		}
		for (int i = 1; i <= 4; i++)
		{
			double temp = CalcuMatrix[i - 1][i - 1];
			for (int j = 1; j <= 8; j++)
			{
				CalcuMatrix[i - 1][j - 1] = CalcuMatrix[i - 1][j - 1] / temp;
			}
		}
		for (int k = 4 - 1; k >= 1; k--)
		{
			for (int i = k; i >= 1; i--)
			{
				mik = CalcuMatrix[i - 1][k];
				for (int j = k + 1; j <= 8; j++)
				{
					CalcuMatrix[i - 1][j - 1] -= mik * CalcuMatrix[k][j - 1];
				}
			}
		}
		double InverseMatrix[4][4];
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				InverseMatrix[i][j] = CalcuMatrix[i][j + 4];
			}
		}

		matrix4f Result;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (fabs(InverseMatrix[i][j]) < 0.0000001)
					InverseMatrix[i][j] = 0.00;
				Result.m[i][j] = InverseMatrix[i][j];
			}
		}
		return Result;
	}


};

inline vec4 operator*( const matrix4f& m, const vec4& v)
{
	return vec4(
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + v.w * m.m[3][0],
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + v.w * m.m[3][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + v.w * m.m[3][2],
		v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + v.w * m.m[3][3]
	);
}

inline vec4& operator *=(vec4& v, const matrix4f& m)
{
	return (v = m * v);
}

inline matrix4f mul(matrix4f& m, matrix4f& n)
{
	return m * n;
}




#endif // !MATRIX_H