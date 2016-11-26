#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <memory>

using Microsoft::WRL::ComPtr;

class Rect;
class Direct3D;
class Window;

constexpr float g_backgroundcolor[]{ .2f, .15f, .1f, 1.f };

class Graphics
{
public:
	Graphics(Window &Win, Direct3D &D3D);
	~Graphics();

	void BeginFrame( );
	void EndFrame( );

	void PutPixel( int X, int Y, unsigned Color );
	void DrawLine( int X0, int Y0, int X1, int Y1, unsigned Color );

	void FillBox( const Rect &Box, unsigned Color );
	void FillCircle( int X, int Y, int Radius, unsigned Color );
	void OutlineBox( const Rect &Box, unsigned Color );
	void OutlineCircle( int X, int Y, int Radius, unsigned Color );

private:
	Direct3D &m_Direct3D;

	ComPtr<ID3D11Texture2D>					m_pSysTexture;
	ComPtr<ID3D11ShaderResourceView>		m_pShaderResourceView;
	ComPtr<ID3D11SamplerState>				m_pSamplerState;
	ComPtr<ID3D11VertexShader>				m_pVertexShader;
	ComPtr<ID3D11PixelShader>				m_pPixelShader;
	ComPtr<ID3D11InputLayout>				m_pLayout;
	ComPtr<ID3D11Buffer>					m_pQuadVertexBuffer, m_pQuadIndexBuffer;
	D3D11_VIEWPORT							m_viewport{};

	std::unique_ptr<unsigned[]> m_pSysbuffer;
public:
	static int m_screenWidth;
	static int m_screenHeight;
};

