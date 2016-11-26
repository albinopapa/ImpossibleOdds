#pragma once

#include "Direct3D.h"
#include "Graphics.h"
#include "Keyboard.h"

#include "MathUtilities.h"

#include "Camera.h"

#include "StarFieldController.h"
#include "StarFieldView.h"

#include "MeteorsController.h"
#include "MeteorsView.h"

class System
{
public:
	System(class Window &Win);
	~System();

	int Run( );
	void UpdateFrame();
	void ComposeFrame( );

	Keyboard &GetKeybaord();

private:
	Window &m_win;
	Direct3D m_direct3D;
	Graphics m_graphics;
	Keyboard m_keyboard;

	// TODO: Make map class
	Vec2f m_minBounds, m_maxBounds;

	// TODO: Decide if MVC needs to be implemented for camera
	Camera m_cam;

	StarFieldController m_starfieldController;
	StarFieldView m_starfieldView;

	MeteorsController m_meteorsController;
	MeteorsView m_meteorsView;
};

