#define WIN32_LEAN_AND_MEAN

#include <string>
#include <sstream>

#include <intrin.h>
#include <memory>
#include "Window.h"
#include "System.h"


System *g_pSystem = nullptr;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, INT )
{	
	Window win;
	if ( !win.GetHandle() )
	{
		return GetLastError();
	}

	std::unique_ptr<System> pSystem;
	try
	{
		pSystem = std::make_unique<System>( win );
	}
	catch ( HRESULT hr )
	{
		return hr;
	}
	g_pSystem = pSystem.get();

	win.Show();

	MSG msg{};	
	while ( true )
	{
		const auto quit = win.ProcessMessageQueue( msg );
		if ( quit == TRUE ) break;
		pSystem->Run( );
	}

	return 0;
}

#include "Keyboard.h"

LRESULT CALLBACK MsgProc( HWND WinHandle, UINT Message, WPARAM wParam, LPARAM lParam )
{
	LRESULT result = 0;

	switch ( Message )
	{
		case WM_CLOSE:
			DestroyWindow( WinHandle );
			break;
		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;
		case WM_KEYDOWN:
			if ( wParam == VK_ESCAPE ) 
				SendMessage( WinHandle, WM_CLOSE, 0, 0 );
			else
				g_pSystem->GetKeybaord().OnKeyPress( static_cast< unsigned char >( wParam ) );
			break;
		case WM_KEYUP:
			g_pSystem->GetKeybaord().OnKeyRelease( static_cast< unsigned char >( wParam ) );
			break;
	}

	result = DefWindowProc( WinHandle, Message, wParam, lParam );

	return result;
}

