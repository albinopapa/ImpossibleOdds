#pragma once

#include "Meteors.h"

class MeteorsController
{
public:
	MeteorsController( unsigned Count, const Vec2f &MinBounds, const Vec2f &MaxBounds );
	~MeteorsController();

	void Update( const Vec2f &MinBounds, const Vec2f &MaxBounds );

	float *GetX()const;
	float *GetY()const;
	float *GetRadius()const;
	unsigned GetCount()const;
	unsigned *GetColor()const;

private:
	void UpdateAcceleration( const Vec2f &MinBounds, const Vec2f &MaxBounds );
private:
	Meteors m_meteors;
};

