#pragma once

#include <Windows.h>

// Rect is just a C++ wrapper for Windows RECT struct for constructors and GetWidth/GetHeight functions
class Rect :public RECT
{
public:
	constexpr explicit Rect()
		:
		RECT( { 0, 0, 0, 0 } )
	{}
	constexpr explicit Rect( long Left, long Top, long Right, long Bottom )
		:
		RECT( { Left, Top, Right, Bottom } )
	{}
	constexpr long GetWidth()const
	{
		return right - left;
	}
	constexpr long GetHeight()const
	{
		return bottom - top;
	}
};

LRESULT CALLBACK MsgProc( HWND WinHandle, UINT Message, WPARAM wParam, LPARAM lParam );
extern const Rect g_winrect;

class Window
{
	// Registers the WNDCLASSEX upon construction and unregisters it upon destruction
	class WinClassManager
	{
	public:
		WinClassManager( const WNDCLASSEX &Wc )
			:
			m_atom( RegisterClassEx( &Wc ) )
		{

		}
		~WinClassManager()
		{
			UnregisterClass( MAKEINTATOM( m_atom ), GetModuleHandle( nullptr ) );
		}

	private:
		ATOM m_atom = 0;
	};

public:
	Window();
	~Window();
	HINSTANCE GetInstance();
	BOOL ProcessMessageQueue( MSG &Msg );
	HWND GetHandle()const;
	LONG GetWidth()const;
	LONG GetHeight()const;
	void Show()const;
private:
	WinClassManager winmanager;
	LONG m_width = 0l, m_height = 0l;
	HWND m_hWnd = nullptr;
};

