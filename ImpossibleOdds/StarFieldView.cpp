#include "StarFieldView.h"
#include "StarFieldController.h"
#include <intrin.h>
#include "Window.h"
#include "Graphics.h"

using float4 = __m128;

StarFieldView::StarFieldView( StarFieldController &Controller )
	:
	m_controller( Controller )
{}


StarFieldView::~StarFieldView()
{}



void StarFieldView::Draw( Graphics & Gfx )
{
	const auto count = m_controller.GetCount();
	const float* sx = m_controller.GetStarsX();
	const float* sy = m_controller.GetStarsY();

	for ( int i = 0; i < count; ++i, ++sx, ++sy )
	{
		Gfx.PutPixel( *sx, *sy, 0xFFFFFFFFu );
	}
}
