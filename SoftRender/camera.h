#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>

#include "matrix.h"
#include "ConstUtil.h"

class Camera {
public:
	Camera(	point lookfrom,	point lookat,vec4 up): _pos(lookfrom),_target(lookat),_up(up),_postemp(lookfrom),_targettmp(lookat) {}

	virtual ~Camera() noexcept{}

	void setPos(const point& pos)
	{
		_pos = pos;
		calcMatrix();
	}

	void setPerspective(float fov, float aspect, float nearZ, float farZ)
	{
		_fov = fov;
		_aspect = aspect;
		_nearZ = nearZ;
		_farZ = farZ;
		calcMatrix();
	}

	//缩放
	void zoom(short wParam)
	{
		float t = 0.95f;
		if (wParam < 0) t = 1.05f;
		_pos.x *= t;
		_pos.y *= t;
		_pos.z *= t;
		calcMatrix();
	}

	//重置相机
	void reset(){
		_pos = _postemp;
		_target = _targettmp;
		calcMatrix();
	}


	//旋转
	void circle(short xMove, short yMove,float sensitivity)
	{

		float xoffset = (float)xMove*sensitivity;
		float yoffset = (float)yMove*sensitivity;

		YAW -= xoffset;
		PITCH -= yoffset;

		if (PITCH > 89.0f)
			PITCH = 89.0f;
		if (PITCH < -89.0f)
			PITCH = -89.0f;

		vec4 dir;
		dir.x = std::cos(D2R(YAW)) * std::cos(D2R(PITCH));
		dir.y = std::sin(D2R(PITCH));
		dir.z = std::sin(D2R(YAW)) * std::cos(D2R(PITCH));

		dir = dir.normalize();
		_target = _pos + dir;

		calcMatrix();

	}

	//相机往前移动
	void front()
	{
		float t = 0.2f;
		vec4 dir = _target - _pos;
		_pos = _pos + t *dir.normalize();
		_target = _target + t * dir.normalize();
		calcMatrix();
	}

	//相机往后移动
	void back()
	{
		float t = 0.2f;
		vec4 dir = _target - _pos;
		_pos = _pos - t *dir.normalize();
		_target = _target - t * dir.normalize();
		calcMatrix();
	}

	//相机往左移动
	void left()
	{
		float t = 0.2f;
		vec4 dir = (_target - _pos).cross(_up);
		_pos = _pos + t * dir.normalize();
		_target = _target + t * dir.normalize();
		calcMatrix();
	}
	//相机往右移动
	void right()
	{
		float t = 0.2f;
		vec4 dir = (_target - _pos).cross(_up);
		_pos = _pos - t * dir.normalize();
		_target = _target - t * dir.normalize();
		calcMatrix();
	}

public:

	matrix4f WorldToPerspective;


private:
	void calcMatrix() {
		vec4 dir = _target - _pos;
		vec4 w = dir.normalize();
		vec4 u =_up.cross(w).normalize();
		vec4 v = w.cross(u).normalize();

		WorldToView = { u.x,v.x,w.x,0.0f,
					u.y,v.y,w.y,0.0f,
					u.z,v.z,w.z,0.0f,
					-_pos.dot(u),-_pos.dot(v),-_pos.dot(w),1.0f };


		float f = 1.0f / (float)tan(_fov * 0.5f);

		ViewToPerspective = { f / _aspect, 0.0f, 0.0f, 0.0f,
				0.0f, f, 0.0f, 0.0f,
				0.0f, 0.0f, _farZ / (_farZ - _nearZ), 1.0f,
				0.0f, 0.0f, -_nearZ * _farZ / (_farZ - _nearZ), 0.0f };

		WorldToPerspective = mul(WorldToView,ViewToPerspective);

	}


public:
	point _pos;
	point _postemp;
	point _targettmp;
	point _target;
	vec4 _up;
	float _fov;
	float _aspect;
	float _nearZ;
	float _farZ;
	float _curXRand = 0.0f;
	float YAW = -90.0f;
	float PITCH = 0.0f;

	matrix4f WorldToView;
	matrix4f ViewToPerspective;
};





#endif // !CAMERA_H
