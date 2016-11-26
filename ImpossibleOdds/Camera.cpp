#include "Camera.h"
#include "Keyboard.h"
#include <Windows.h>

Camera::Camera( float X, float Y )
	:
	m_position( X, Y )
{}


Camera::~Camera()
{}

void Camera::Update( Keyboard & Kbd )
{
	const auto pos = Vec2f{ [ &Kbd ]
	{
		if ( Kbd.KeyPressed( VK_LEFT ) )
		{
			return -1.f;
		}
		else if ( Kbd.KeyPressed( VK_RIGHT ) )
		{
			return 1.f;
		}
		return 0.f;
	}( ), [ &Kbd ]
	{
		if ( Kbd.KeyPressed( VK_UP ) )
		{
			return -1.f;
		}
		else if ( Kbd.KeyPressed( VK_DOWN ) )
		{
			return 1.f;
		}
		return 0.f;
	}( ) };
	const auto cam_speed = m_speed;

	if ( !( pos.x == 0.f && pos.y == 0.f ) )
	{
		m_position = m_position + Normalize( pos ) * cam_speed;
	}	
}

const Vec2f & Camera::GetPosition() const
{
	return m_position;
}
