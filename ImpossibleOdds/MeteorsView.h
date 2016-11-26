#pragma once

#include <vector>

class MeteorsController;
class Graphics;
class Camera;

class MeteorsView
{
public:
	MeteorsView(MeteorsController &Controller);
	~MeteorsView();

	void Update( const Camera &Cam );
	void Draw( Graphics &Gfx );

private:
	void ClearCaches();

private:
	MeteorsController &m_controller;
	std::vector<float> m_x, m_y, m_radius;
	std::vector<unsigned> m_color;
};

