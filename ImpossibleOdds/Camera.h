#pragma once

#include "MathUtilities.h"

class Keyboard;

class Camera
{
public:
	Camera(float X, float Y);
	~Camera();

	void Update(Keyboard &Kbd);
	const Vec2f &GetPosition()const;

private:
	Vec2f m_position;
	float m_speed = 2.f;
};

