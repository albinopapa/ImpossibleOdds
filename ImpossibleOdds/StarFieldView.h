#pragma once

class StarFieldController;

class StarFieldView
{
public:
	StarFieldView( StarFieldController &pStarField );
	~StarFieldView();

	void Draw( class Graphics &Gfx );

private:
	StarFieldController &m_controller;
};

