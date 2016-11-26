#include "Keyboard.h"



Keyboard::Keyboard()
{}


Keyboard::~Keyboard()
{}

bool Keyboard::KeyPressed( unsigned char Key )
{
	return m_keys[Key];
}

bool Keyboard::KeyReleased( unsigned char Key )
{
	return m_keys[ Key ];
}

void Keyboard::OnKeyPress( unsigned char Key )
{
	m_keys[ Key ] = true;
}

void Keyboard::OnKeyRelease( unsigned char Key )
{
	m_keys[ Key ] = false;
}
