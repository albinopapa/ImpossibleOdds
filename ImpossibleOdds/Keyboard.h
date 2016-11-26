#pragma once

class System;

class Keyboard
{
public:
	Keyboard();
	~Keyboard();

	bool KeyPressed( unsigned char Key );
	bool KeyReleased( unsigned char Key );
	void OnKeyPress( unsigned char Key );
	void OnKeyRelease( unsigned char Key );

private:
	bool m_keys[ 256 ]{};
};

