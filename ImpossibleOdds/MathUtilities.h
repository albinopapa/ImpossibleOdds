#pragma once

#include <math.h>

struct Vec2f
{
	float x = 0.f, y = 0.f;

	constexpr Vec2f() {}
	constexpr Vec2f( float X, float Y ) :x( X ), y( Y ) {}

	Vec2f constexpr operator+( const Vec2f &V )const
	{
		return{ x + V.x, y + V.y };
	}
	Vec2f constexpr operator-()const
	{
		return{ -x, -y };
	}
	Vec2f constexpr operator-( const Vec2f &V )const
	{
		return{ x - V.x, y - V.y };
	}
	Vec2f constexpr operator*( const float S )const
	{
		return{ x * S, y * S };
	}
};


constexpr auto sqSec = .016f * .016f;
constexpr float grav = ( 9.8f * sqSec );

constexpr float Sq( const float A )
{
	return A * A;
}

constexpr float DotProduct( const Vec2f &V0, const Vec2f &V1 )
{
	return ( ( V0.x * V1.x ) + ( V0.y * V1.y ) );
}

constexpr float SqMagnitude( const Vec2f &V )
{
	return DotProduct( V, V );
}

inline float Magnitude( const Vec2f &V )
{
	return sqrt( SqMagnitude( V ) );
}

inline float SqDistance( const Vec2f &V0, const Vec2f &V1 )
{	
	return SqMagnitude( V1 - V0 );
}

inline float Distance( const Vec2f &V0, const Vec2f &V1 )
{
	return Magnitude( V1 - V0 );
}

constexpr float Recip( const float Val )
{
	return 1.f / Val;
}

inline float RecipSqrt( const float SqA )
{
	return Recip( sqrt( SqA ) );
}

inline Vec2f Normalize( const Vec2f &V )
{
	const auto recipMag = RecipSqrt( SqMagnitude( V ) );
	return{ V.x * recipMag, V.y * recipMag };
}

inline Vec2f Normalize( const Vec2f &V, const float SqMag )
{
	const auto recipMag = RecipSqrt( SqMag );
	return{ V.x * recipMag, V.y * recipMag };
}

inline Vec2f CalculateGravForce( const Vec2f &PosI, const Vec2f &PosJ, float MassI, float MassJ )
{
	const auto norm = Normalize( PosJ - PosI );
	const auto combinedMass = MassJ + MassI;
	const auto force = grav * combinedMass;
	const auto accel = norm * force;
	return accel;
}

inline Vec2f ClampVelocity( const Vec2f &Vel, float MaxSpeed )
{
	const auto sqSpeed = SqMagnitude( Vel );
	const auto sqMaxSpeed = Sq( MaxSpeed );
	if ( sqSpeed > sqMaxSpeed )
	{
		const auto norm = Normalize( Vel, sqSpeed );
		const auto correctedVel = norm * MaxSpeed;
		return correctedVel;
	}
	return Vel;
}