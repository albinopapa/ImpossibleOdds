#include "System.h"
#include "Window.h"
#include <algorithm>

System::System( Window &Win )
	:
	m_win( Win ),
	m_direct3D( m_win ),
	m_graphics( m_win, m_direct3D ),
	m_minBounds( { -10000, -10000 } ),
	m_maxBounds( { 10000, 10000 } ),
	m_cam( 0.f, 0.f ),
	m_starfieldController( 2500, m_cam ),
	m_starfieldView( m_starfieldController ),
	m_meteorsController( 500, m_minBounds, m_maxBounds ),
	m_meteorsView( m_meteorsController )
{	
}

System::~System()
{}

int System::Run( )
{
	UpdateFrame();

	m_graphics.BeginFrame( );
	ComposeFrame( );
	m_graphics.EndFrame( );
	return 0;
}

void System::UpdateFrame()
{
	m_cam.Update( m_keyboard );
	m_starfieldController.Update( m_cam );
	m_meteorsController.Update( m_minBounds, m_maxBounds );
	m_meteorsView.Update( m_cam );
}

void System::ComposeFrame( )
{	
	m_starfieldView.Draw( m_graphics );
	m_meteorsView.Draw( m_graphics );
}

Keyboard & System::GetKeybaord()
{
	return m_keyboard;
}
