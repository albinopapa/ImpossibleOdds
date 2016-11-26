#pragma once

// Includes
#include "StarField.h"
#include "MathUtilities.h"

// Forward declarations
class Camera;

class StarFieldController
{
public:
	StarFieldController( unsigned StarCound, const Camera &Cam );
	~StarFieldController();
	
	void Update( const Camera &Cam );

	const float *GetStarsX()const;
	const float *GetStarsY()const;
	unsigned GetCount()const;

private:
	StarField m_starfield;
	Vec2f m_prevCamPosition;
};

