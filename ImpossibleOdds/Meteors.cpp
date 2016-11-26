#include "Meteors.h"
#include <random>
#include "SIMDUtils.h"
#include "Window.h"
#include "MathUtilities.h"

Meteors::Meteors( unsigned Count, const Vec2f &MinBounds, const Vec2f &MaxBounds )
	:
	m_count( Count ),
	m_x( std::make_unique<float[]>( m_count ) ),
	m_y( std::make_unique<float[]>( m_count ) ),
	m_vx( std::make_unique<float[]>( m_count ) ),
	m_vy( std::make_unique<float[]>( m_count ) ),
	m_ax( std::make_unique<float[]>( m_count ) ),
	m_ay( std::make_unique<float[]>( m_count ) ),
	m_mass( std::make_unique<float[]>( m_count ) ),
	m_radius( std::make_unique<float[]>( m_count ) ),
	m_color( std::make_unique<unsigned[]>( m_count ) )
{
	const auto massCoeff = .0001f;
	std::mt19937 rng;

	const auto numClusters = 5u;
	const auto clusterRadius = 1000.f;
	const auto meteorsInCluster = m_count / numClusters;

	const auto pi = 3.141592f;
	const auto rad = pi / 180.f;
	/*const auto width = static_cast<float>( g_winrect.GetWidth() );
	const auto height = static_cast<float>( g_winrect.GetHeight() );*/
	const auto width = static_cast<float>( MaxBounds.x - MinBounds.x );
	const auto height = static_cast<float>( MaxBounds.y - MinBounds.y );

	for ( unsigned j = 0; j < numClusters; ++j )
	{
		const auto groupStart = j * meteorsInCluster;
		const auto groupEnd = groupStart + meteorsInCluster;
		
		std::uniform_real_distribution<float> xDist( -clusterRadius, clusterRadius );
		std::uniform_real_distribution<float> yDist( -clusterRadius, clusterRadius );
		std::uniform_real_distribution<float> radiusDist( 5.f, 15.f );

		const auto outerStep = 360.f / static_cast< float >( numClusters );
		const auto outerAngle = static_cast< float >( j ) * outerStep * rad;
		const auto xOffset = cos( outerAngle ) * ( width * .5f );
		const auto yOffset = sin( outerAngle ) * ( height * .5f );

		const auto step = 360.f / static_cast< float >( meteorsInCluster );
		for ( unsigned i = groupStart; i < groupEnd; ++i )
		{
			const auto angle = static_cast<float>( i ) * step * rad;
			const auto cosine = cos( angle );
			const auto sine = sin( angle );
			
			const auto x = ( cosine * xDist( rng ) ) + xOffset;
			const auto y = ( sine * yDist( rng ) ) + yOffset;

			m_x[ i ] = x;
			m_y[ i ] = y;
			m_radius[ i ] = radiusDist( rng );
			m_mass[ i ] = m_radius[ i ] * massCoeff;
			m_color[ i ] = 0xFFC86432u;

			if ( i > 0 )
			{
				const auto dir = Normalize( Vec2f{ m_y[ i ] - yOffset, -m_x[ i ] - xOffset } );
				m_vx[ i ] = dir.x;
				m_vy[ i ] = dir.y;
			}
		}
	}
}


Meteors::~Meteors()
{}

