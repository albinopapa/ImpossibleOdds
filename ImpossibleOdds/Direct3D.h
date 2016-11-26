#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl\client.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

// Shorthand for using namespace
using Microsoft::WRL::ComPtr;

class Direct3D
{
public:
	Direct3D(class Window &Win );
	~Direct3D();

	ID3D11Device *GetDevice()const;
	ID3D11DeviceContext *GetContext()const;
	ID3D11RenderTargetView *GetRenderTarget()const;
	IDXGISwapChain *GetSwapchain()const;
	D3D_FEATURE_LEVEL GetFeatureLevel()const;
	
private:

	// D3D11 interfaces
	ComPtr<ID3D11Device>					m_pDevice;
	ComPtr<ID3D11DeviceContext>				m_pContext;
	ComPtr<IDXGISwapChain>					m_pSwapchain;
	ComPtr<ID3D11RenderTargetView>			m_pRenderTarget;
	D3D_FEATURE_LEVEL						m_featurelevel{};
};

