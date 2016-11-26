#include "Window.h"
#include "Graphics.h"


// A function to return a Rect instead of having to construct one and pass it in.
Rect GetRectFromHandle( HWND WinHandle )
{
	Rect r{};
	GetClientRect( WinHandle, &r );
	return r;
}

// Might not need this but it's here, global Rect for desktop client rect size
const Rect g_winrect = GetRectFromHandle( GetDesktopWindow() );


constexpr auto g_wndClassExSize = sizeof( WNDCLASSEX );
constexpr auto g_wndClassExStyle = 0u;
constexpr auto g_wndClassProcFn = MsgProc;
constexpr auto g_wndClassExtraClassMem = 0u;
constexpr auto g_wndClassExtraWindowMem = 0u;
constexpr auto g_wndClassBigIcon = nullptr;
constexpr auto g_wndClassSmallIcon = nullptr;
constexpr auto g_wndClassCursor = nullptr;
constexpr auto g_wndClassBrush = reinterpret_cast<HBRUSH>( COLOR_WINDOW - 1u );
constexpr auto g_wndClassMenuName = nullptr;
constexpr auto g_wndClassClassName = L"MyClass";


Window::Window()
	:
	winmanager( { g_wndClassExSize, g_wndClassExStyle, g_wndClassProcFn, g_wndClassExtraClassMem,
				g_wndClassExtraWindowMem, GetInstance(), g_wndClassBigIcon, g_wndClassCursor, g_wndClassBrush,
				g_wndClassMenuName, g_wndClassClassName, g_wndClassSmallIcon } ),
	m_width( g_winrect.GetWidth() ),
	m_height( g_winrect.GetHeight() )
{
	// Set the style of window to WS_POPUP to take up the entire screen, set extended style to 0
	const auto winstyle = WS_POPUP;
	const auto exwinstyle = 0;

	// Create the window
	m_hWnd = CreateWindowEx(
		exwinstyle, g_wndClassClassName, L"ImpossibleOdds", winstyle,
		0, 0, m_width, m_height,
		nullptr, nullptr, GetInstance(), nullptr
	);
}


Window::~Window()
{}

HINSTANCE Window::GetInstance()
{
	return GetModuleHandle( nullptr );
}

BOOL Window::ProcessMessageQueue( MSG &Msg )
{
	while ( PeekMessage( &Msg, nullptr, 0, 0, PM_REMOVE ) )
	{
		TranslateMessage( &Msg );
		DispatchMessage( &Msg );
	}

	return ( Msg.message == WM_QUIT );
}

HWND Window::GetHandle() const
{
	return m_hWnd;
}

LONG Window::GetWidth() const
{
	return m_width;
}

LONG Window::GetHeight() const
{
	return m_height;
}

void Window::Show()const
{
	ShowWindow( m_hWnd, SW_SHOWDEFAULT );
}
