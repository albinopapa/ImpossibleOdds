#include "StarField.h"
#include <random>
#include "Window.h"

using float4 = __m128;

StarField::StarField( unsigned StarCound, float StarSpeed )
	:
	m_starcount( StarCound ),
	m_x( std::make_unique<float[]>( m_starcount ) ),
	m_y( std::make_unique<float[]>( m_starcount ) ),
	m_starspeed( StarSpeed )
{
	std::mt19937 rng;
	std::uniform_real_distribution<float> xDist( 0.f, static_cast<float>( g_winrect.GetWidth() - 1l ) );
	std::uniform_real_distribution<float> yDist( 0.f, static_cast<float>( g_winrect.GetHeight() - 1l ) );

	for ( int i = 0; i < m_starcount; ++i )
	{
		m_x[ i ] = xDist( rng );
		m_y[ i ] = yDist( rng );
	}
}


StarField::~StarField()
{}

