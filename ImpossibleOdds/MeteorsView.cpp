#include "MeteorsView.h"
#include "MeteorsController.h"
#include "Camera.h"
#include "Graphics.h"
#include "Window.h"


MeteorsView::MeteorsView( MeteorsController &Controller )
	:
	m_controller( Controller )
{}


MeteorsView::~MeteorsView()
{}

void MeteorsView::Update( const Camera &Cam )
{
	ClearCaches();
	const auto &cam = Cam.GetPosition();
	const auto count = m_controller.GetCount();
	const float *mx = m_controller.GetX();
	const float *my = m_controller.GetY();
	const float *mRadius = m_controller.GetRadius();
	const unsigned *mColor = m_controller.GetColor();

	const float width = static_cast< float >( g_winrect.GetWidth() );
	const float height = static_cast< float >( g_winrect.GetHeight() );

	for ( int i = 0; i < count; ++i, ++mx, ++my, ++mRadius, ++mColor )
	{
		const auto px = *mx - cam.x;
		const auto py = *my - cam.y;

		if ( px - *mRadius < width && px + *mRadius >= 0 &&
			 py - *mRadius < height && py + *mRadius >= 0 )
		{
			m_x.push_back( px );
			m_y.push_back( py );
			m_radius.push_back( *mRadius );
			m_color.push_back( *mColor );
		}
	}
}

void MeteorsView::Draw( Graphics & Gfx )
{
	auto cIter = m_color.begin();
	
	for ( auto xIter = m_x.begin(),
		  yIter = m_y.begin(), 
		  radIter = m_radius.begin(),
		  end = m_x.end();
		  xIter != end; ++xIter, ++yIter, ++radIter, ++cIter )
	{
		Gfx.FillCircle(*xIter, *yIter, *radIter, *cIter );
	}
}

void MeteorsView::ClearCaches()
{
	m_x.clear();
	m_y.clear();
	m_radius.clear();
	m_color.clear();
}

