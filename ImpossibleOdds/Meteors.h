#pragma once

#include <memory>

struct Vec2f;

class Meteors
{
public:
	Meteors(unsigned Count, const Vec2f &MinBounds, const Vec2f &MaxBounds );
	~Meteors();

private:
	friend class MeteorsController;
	unsigned m_count;
	std::unique_ptr<float[]> m_x, m_y, m_vx, m_vy, m_ax, m_ay, m_mass, m_radius;
	std::unique_ptr<unsigned[]>m_color;
};

