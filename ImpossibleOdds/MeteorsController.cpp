#include "MeteorsController.h"
#include "MathUtilities.h"

constexpr auto maxspeed = 5.f;
constexpr auto sqMaxSpeed = Sq( maxspeed );

MeteorsController::MeteorsController( unsigned Count, const Vec2f &MinBounds, const Vec2f &MaxBounds )
	:
	m_meteors( Count, MinBounds, MaxBounds )
{}

void MeteorsController::Update( const Vec2f &MinBounds, const Vec2f &MaxBounds )
{
	UpdateAcceleration( MinBounds, MaxBounds );
}


MeteorsController::~MeteorsController()
{}

float * MeteorsController::GetX() const
{
	return m_meteors.m_x.get();
}

float * MeteorsController::GetY() const
{
	return m_meteors.m_y.get();
}

float * MeteorsController::GetRadius() const
{
	return m_meteors.m_radius.get();
}

unsigned MeteorsController::GetCount() const
{
	return m_meteors.m_count;
}

unsigned * MeteorsController::GetColor() const
{
	return m_meteors.m_color.get();
}

void MeteorsController::UpdateAcceleration( const Vec2f &MinBounds, const Vec2f &MaxBounds )
{
	float *ppx = m_meteors.m_x.get();
	float *ppy = m_meteors.m_y.get();
	float *pax = m_meteors.m_ax.get();
	float *pay = m_meteors.m_ay.get();
	float *pvx = m_meteors.m_vx.get();
	float *pvy = m_meteors.m_vy.get();
	float *pmass = m_meteors.m_mass.get();
	float *prad = m_meteors.m_radius.get();
	const auto count = m_meteors.m_count;

	for ( int j = 0; j < count; ++j )
	{
		Vec2f posj = { ppx[ j ], ppy[ j ] };
		Vec2f velj = { pvx[ j ], pvy[ j ] };
		Vec2f accj = { pax[ j ], pay[ j ] };

		for ( int i = j + 1; i < count; ++i )
		{
			Vec2f posi = { ppx[ i ], ppy[ i ] };
			Vec2f acci = { pax[ i ], pay[ i ] };
			const Vec2f dirFromJtoI = Normalize( posi - posj );

			// Calculate gravity force			
			const auto forceI = grav * pmass[ i ];
			auto accelI = dirFromJtoI * forceI;
			const auto forceJ = grav * pmass[ j ];
			auto accelJ = dirFromJtoI * forceJ;
			acci = acci - accelJ;
			accj = accj + accelI;

			pax[ i ] = acci.x;
			pay[ i ] = acci.y;
			pax[ j ] = accj.x;
			pay[ j ] = accj.y;
		}

		velj = velj + accj;
		posj = posj + velj;
		accj = { 0.f, 0.f };

		pvx[ j ] = velj.x;
		pvy[ j ] = velj.y;

		ppx[ j ] = posj.x;
		ppy[ j ] = posj.y;

		ppx[ j ] = ( ppx[ j ] < MinBounds.x && pvx[ j ] < 0.f ) ?
			ppx[ j ] = MaxBounds.x : ppx[ j ] > MaxBounds.x && pvx[ j ] > 0.f ?
			ppx[ j ] = MinBounds.x : ppx[ j ];
		ppy[ j ] = ( ppy[ j ] < MinBounds.y && pvy[ j ] < 0.f ) ?
			ppy[ j ] = MaxBounds.y : ppy[ j ] > MaxBounds.y && pvy[ j ] > 0.f ?
			ppy[ j ] = MinBounds.y : ppy[ j ];

		pax[ j ] = accj.x;
		pay[ j ] = accj.y;
	}
}
