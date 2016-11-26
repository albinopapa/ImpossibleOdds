#include "Direct3D.h"
#include "Window.h"

#include <string>
#include <sstream>


Direct3D::Direct3D( Window &Win )
{
	const auto screenWidth = static_cast<UINT>( Win.GetWidth() );
	const auto screenHeight = static_cast<UINT>( Win.GetHeight() );

	// Pre-setup
	D3D11_CREATE_DEVICE_FLAG flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	const auto bufferDesc = DXGI_MODE_DESC{
		screenWidth,							// Width
		screenHeight,							// Height
		DXGI_RATIONAL{ 60, 1 },					// Refresh rate
		DXGI_FORMAT_R8G8B8A8_UNORM,				// Buffer format
		DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,	// Scanline order
		DXGI_MODE_SCALING_UNSPECIFIED };		// Scaling

	DXGI_SWAP_CHAIN_DESC scd{
		bufferDesc,								// Buffer description
		DXGI_SAMPLE_DESC{ 1, 0 },				// Count, Quality
		DXGI_USAGE_RENDER_TARGET_OUTPUT,		// Resource usage
		1,										// buffer count
		Win.GetHandle(),						// Window handle
		TRUE,									// Windowed?
		DXGI_SWAP_EFFECT_DISCARD,				// Swap effect
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH	// Flags
	};

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,								// IDXGIAdapter *
		D3D_DRIVER_TYPE_HARDWARE,				// Driver type
		nullptr,								// Software driver implementation
		flags,									// Creation flags (BGRA support and Debug are defaults)
		nullptr,								// Feature level list, nullptr for default array
		0,										// Feature level list count
		D3D11_SDK_VERSION,						// SDK version
		&scd,									// Address of swapchain description
		&m_pSwapchain,							// Address of swapchain interface
		&m_pDevice,								// Address of D3D11 device interface
		&m_featurelevel,						// Address of returned feature level
		&m_pContext								// Address of D3D11 device context interface
	);

	if ( FAILED( hr ) )
	{
		MessageBox( nullptr, L"Failed to create device, context and/or swapchain.", L"Device creation error.", MB_OK );
		throw( hr );
	}

	ComPtr<ID3D11Texture2D>	pBackbuffer;
	// Initialize the back buffer
	hr = m_pSwapchain->GetBuffer( 0, IID_PPV_ARGS( &pBackbuffer ) );
	if ( FAILED( hr ) )
	{
		MessageBox( nullptr, L"Failed to get the back buffer.", L"Back buffer initialization error.", MB_OK );
		throw( hr );
	}

	// Create and set the render target using the back buffer
	hr = m_pDevice->CreateRenderTargetView( pBackbuffer.Get(), nullptr, &m_pRenderTarget );
	if ( FAILED( hr ) )
	{
		MessageBox( nullptr, L"Failed to create render target.", L"RenderTargetView creation error.", MB_OK );
		throw( hr );
	}
}

Direct3D::~Direct3D()
{}

ID3D11Device * Direct3D::GetDevice() const
{
	return m_pDevice.Get();
}

ID3D11DeviceContext * Direct3D::GetContext() const
{	
	return m_pContext.Get();
}

ID3D11RenderTargetView * Direct3D::GetRenderTarget() const
{
	return m_pRenderTarget.Get();
}

IDXGISwapChain * Direct3D::GetSwapchain() const
{
	return m_pSwapchain.Get();
}

D3D_FEATURE_LEVEL Direct3D::GetFeatureLevel() const
{
	return m_featurelevel;
}
