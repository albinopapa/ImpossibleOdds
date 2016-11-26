#pragma once

#include <memory>

class StarField
{
public:
	StarField(unsigned StarCound, float StarSpeed);
	~StarField();

private:
	friend class StarFieldController;

	unsigned m_starcount = 0u;
	std::unique_ptr<float[]> m_x, m_y;
	float m_starspeed = 0.f;
};

