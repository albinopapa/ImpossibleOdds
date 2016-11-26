#include "StarFieldController.h"
#include "Window.h"
#include "Camera.h"


StarFieldController::StarFieldController( unsigned StarCound, const Camera &Cam )
	:
	m_starfield( StarCound, 0.f ),
	m_prevCamPosition( Cam.GetPosition() )
{}


StarFieldController::~StarFieldController()
{}

void StarFieldController::Update( const Camera & Cam )
{
	const auto &curCamPosition = Cam.GetPosition();
	const auto offset = m_prevCamPosition - curCamPosition;
	m_prevCamPosition = curCamPosition;

	auto sx = m_starfield.m_x.get();
	auto sy = m_starfield.m_y.get();

	const auto width = static_cast<float>( g_winrect.GetWidth() );
	const auto height = static_cast< float >( g_winrect.GetHeight() );

	for ( int i = 0; i < m_starfield.m_starcount; ++i, ++sx, ++sy )
	{
		*sx += offset.x;
		*sy += offset.y;

		if ( *sx < 0.f )
		{
			*sx = width - 1.f;
		}
		else if ( *sx >= width )
		{
			*sx = 0.f;
		}
		if ( *sy < 0.f )
		{
			*sy = ( height - 1.f );
		}
		else if ( *sy >= height )
		{
			*sy = 0.f;
		}
	}

}

const float * StarFieldController::GetStarsX() const
{
	return m_starfield.m_x.get();
}

const float * StarFieldController::GetStarsY() const
{
	return m_starfield.m_y.get();
}

unsigned StarFieldController::GetCount() const
{
	return m_starfield.m_starcount;
}
