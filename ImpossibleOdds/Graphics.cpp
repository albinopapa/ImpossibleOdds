#include "Graphics.h"
#include "Direct3D.h"
#include "Window.h"
#include "SimpleModel.h"
#include <vector>
#include <sstream>



int Graphics::m_screenWidth = g_winrect.GetWidth();
int Graphics::m_screenHeight= g_winrect.GetHeight();

Graphics::Graphics( Window &Win, Direct3D &D3D )
	:
	m_Direct3D(D3D)
{
	m_screenWidth = Win.GetWidth();
	m_screenHeight = Win.GetHeight();
	const auto pDevice = m_Direct3D.GetDevice();

	D3D11_TEXTURE2D_DESC td{
		static_cast<UINT>( m_screenWidth ),
		static_cast<UINT>( m_screenHeight ),
		1u,
		1u,
		DXGI_FORMAT_B8G8R8A8_UNORM,
		{ 1u, 0u },
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_SHADER_RESOURCE,
		D3D11_CPU_ACCESS_WRITE,
		0u
	};

	{
		const auto hr = pDevice->CreateTexture2D( &td, nullptr, &m_pSysTexture );
		if ( FAILED( hr ) )
		{
			MessageBox( nullptr, L"Failed to create system texture.", 
						L"System texture creation error.", MB_OK );
			throw( hr );
		}
	}

	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srv{};
		srv.Format = td.Format;
		srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srv.Texture2D = { 0, 1 };

		const auto hr = pDevice->CreateShaderResourceView( m_pSysTexture.Get(), &srv, &m_pShaderResourceView );
		if ( FAILED( hr ) )
		{
			MessageBox( nullptr, L"Failed to create shader resource view for the system texture.", L"Shader resource view for the system texture creation error.", MB_OK );
			throw( hr );
		}
	}

	{
		FLOAT borderColor[] = { 0.f, 0.f, 0.f, 0.f };
		D3D11_SAMPLER_DESC sampDesc{
			D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT,
			D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP,
			D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP,
			D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP,
			0.f,
			1,
			D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER,
			borderColor[ 0 ],
			borderColor[ 1 ],
			borderColor[ 2 ],
			borderColor[ 3 ],
			0.f,
			D3D11_FLOAT32_MAX
		};
		const auto hr = pDevice->CreateSamplerState( &sampDesc, &m_pSamplerState );
		if ( FAILED( hr ) )
		{
			MessageBox( nullptr, L"Failed to create sampler state.", L"Sampler state creation error.", MB_OK );
			throw( hr );
		}
	}

	{		
		const SimpleModel::VertexBuffer vertList[] = {
			{ -1.0f, 1.0f, 0.5f, 0.0f, 0.0f },
			{ 1.0f, 1.0f, 0.5f, 1.0f, 0.0f },
			{ -1.0f, -1.0f, 0.5f, 0.0f, 1.0f },
			{ 1.0f, -1.0f, 0.5f, 1.0f, 1.0f }
		};

		const auto bufferSize = sizeof( SimpleModel::VertexBuffer ) * ARRAYSIZE( vertList );
		D3D11_BUFFER_DESC vbdesc{ bufferSize, D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER, 0, 0, 0 };

		D3D11_SUBRESOURCE_DATA vbsrd{ vertList, 0, 0 };

		const auto hr = pDevice->CreateBuffer( &vbdesc, &vbsrd, &m_pQuadVertexBuffer );
		if ( FAILED( hr ) )
		{
			MessageBox( nullptr, L"Failed to create vertex buffer.", L"Vertex buffer creation error.", MB_OK );
			throw( hr );
		}
	}

	{
		const DWORD idxList[] = {
			0, 1, 2,
			3, 2, 1
		};
		D3D11_SUBRESOURCE_DATA ibsrd{
			idxList,
			0,
			0
		};
		D3D11_BUFFER_DESC ibdesc{
			sizeof( DWORD ) * ARRAYSIZE( idxList ),
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_INDEX_BUFFER,
			0,
			0,
			0,
		};


		const auto hr = pDevice->CreateBuffer( &ibdesc, &ibsrd, &m_pQuadIndexBuffer );
		if ( FAILED( hr ) )
		{
			MessageBox( nullptr, L"Failed to create index buffer.", L"Index buffer creation error.", MB_OK );
			throw( hr );
		}
	}

	ComPtr<ID3DBlob> vsBlob;
	{
		const auto hr = D3DReadFileToBlob( L"backbuffer.cso", &vsBlob );

		if ( FAILED( hr ) )
		{
			std::wstringstream ws;
			ws << hr << L"Failed to load vertex shader file.";
			MessageBox( nullptr, ws.str().c_str(), L"Failed to compile vertex shader.", MB_OK );
			throw( hr );
		}
	}
	{
		const auto hr = pDevice->CreateVertexShader(
			vsBlob->GetBufferPointer(),
			vsBlob->GetBufferSize(),
			nullptr,
			&m_pVertexShader );
		if ( FAILED( hr ) )
		{
			MessageBox( nullptr, L"Failed to create vertex shader.", L"Vertex shader creation error.", MB_OK );
			throw( hr );
		}
	}
	ComPtr<ID3DBlob> psBlob;
	{
		const auto hr = D3DReadFileToBlob( L"backbuffer_ps.cso", &psBlob );
		if ( FAILED( hr ) )
		{
			MessageBox( nullptr, L"Failed to create pixel shader.", L"Pixel shader creation error.", MB_OK );
			throw( hr );
		}
	}
	{
		const auto hr = pDevice->CreatePixelShader(
			psBlob->GetBufferPointer(),
			psBlob->GetBufferSize(),
			nullptr,
			&m_pPixelShader
		);
		if ( FAILED( hr ) )
		{
			MessageBox( nullptr, L"Failed to create pixel shader.", L"Pixel shader creation error.", MB_OK );
			throw( hr );
		}
	}
	{
		D3D11_INPUT_ELEMENT_DESC ied[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		const auto hr = pDevice->CreateInputLayout(
			ied,
			ARRAYSIZE( ied ),
			vsBlob->GetBufferPointer(),
			vsBlob->GetBufferSize(),
			&m_pLayout );
		if ( FAILED( hr ) )
		{
			MessageBox( nullptr, L"Failed to create input layout.", L"Input layout creation error.", MB_OK );
			throw( hr );
		}
	}
	{
		m_viewport.TopLeftX = 0.f;
		m_viewport.TopLeftY = 0.f;
		m_viewport.Width = static_cast< float >( m_screenWidth );
		m_viewport.Height = static_cast< float >( m_screenHeight );
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.f;
	}
	m_pSysbuffer = std::make_unique<unsigned[]>( m_screenWidth * m_screenHeight );
}


Graphics::~Graphics()
{}

void Graphics::BeginFrame(  )
{
	memset( m_pSysbuffer.get(), 0x0, m_screenWidth * m_screenHeight * sizeof( unsigned ) );
	m_Direct3D.GetContext()->ClearRenderTargetView( m_Direct3D.GetRenderTarget(), g_backgroundcolor );
}

void Graphics::EndFrame( )
{
	// Update texture
	{
		const auto sysmemPitch = m_screenWidth * sizeof( unsigned );
		const auto sysptr = reinterpret_cast< BYTE* >( m_pSysbuffer.get() );

		D3D11_MAPPED_SUBRESOURCE mr{};
		const auto hr = m_Direct3D.GetContext()->Map( m_pSysTexture.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD,
													  0, &mr );
		if ( FAILED( hr ) )
		{
			std::wstringstream ss;
			ss << hr << L": Failed to update the current frame.";
			MessageBox( nullptr, ss.str().c_str(), L"Map error.", MB_OK );
			throw( hr );
		}

		const auto vidmemPitch = mr.RowPitch;
		const auto vidptr = reinterpret_cast< BYTE* >( mr.pData );

		for ( auto y = 0; y < m_screenHeight; ++y )
		{
			memcpy( &vidptr[ y * vidmemPitch ], &sysptr[ y * sysmemPitch ], sysmemPitch );
		}

		m_Direct3D.GetContext()->Unmap( m_pSysTexture.Get(), 0 );

	}

	// Set pipeline state
	{
		const auto pContext = m_Direct3D.GetContext();
		const auto pRenderTarget = m_Direct3D.GetRenderTarget();

		const unsigned stride = sizeof( SimpleModel::VertexBuffer );
		const auto offset = 0u;

		pContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
		pContext->IASetVertexBuffers( 0u, 1u, m_pQuadVertexBuffer.GetAddressOf(), &stride, &offset );
		pContext->IASetIndexBuffer( m_pQuadIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0 );
		pContext->IASetInputLayout( m_pLayout.Get() );
		pContext->OMSetRenderTargets( 1u, &pRenderTarget, nullptr );
		pContext->PSSetSamplers( 0u, 1u, m_pSamplerState.GetAddressOf() );
		pContext->PSSetShaderResources( 0u, 1u, m_pShaderResourceView.GetAddressOf() );
		pContext->VSSetShader( m_pVertexShader.Get(), nullptr, 0u );
		pContext->PSSetShader( m_pPixelShader.Get(), nullptr, 0u );
		pContext->RSSetViewports( 1u, &m_viewport );

		pContext->DrawIndexed( 6u, 0u, 0u );
	}

	// Present the frame
	{
		const auto hr = m_Direct3D.GetSwapchain()->Present( 1u, 0u );
		if ( FAILED( hr ) )
		{
			if ( hr == DXGI_ERROR_DEVICE_REMOVED )
			{
				throw hr;
			}
			MessageBox( nullptr, L"Failed to present the current frame.", L"Present frame error.", MB_OK );
		}
	}
}

void Graphics::PutPixel( int X, int Y, unsigned Color )
{
	const int idx = X + ( Y * m_screenWidth );
	m_pSysbuffer[ idx ] = Color;
}

void Graphics::DrawLine( int X0, int Y0, int X1, int Y1, unsigned C )
{
	const float dx = X1 - X0;
	const float dy = Y1 - Y0;

	if ( dy == 0.0f && dx == 0.0f )
	{
		PutPixel( int( X0 ), int( Y0 ), C );
	}
	else if ( abs( dy ) > abs( dx ) )
	{
		if ( dy < 0.0f )
		{
			std::swap( X0, X1 );
			std::swap( Y0, Y1 );
		}

		const float m = dx / dy;
		float y = Y0;
		int lastIntY;
		for ( float x = X0; y < Y1; y += 1.0f, x += m )
		{
			lastIntY = int( y );
			PutPixel( int( x ), lastIntY, C );
		}
		if ( int( Y1 ) > lastIntY )
		{
			PutPixel( int( X1 ), int( Y1 ), C );
		}
	}
	else
	{
		if ( dx < 0.0f )
		{
			std::swap( X0, X1 );
			std::swap( Y0, Y1 );
		}

		const float m = dy / dx;
		float x = X0;
		int lastIntX;
		for ( float y = Y0; x < X1; x += 1.0f, y += m )
		{
			lastIntX = int( x );
			PutPixel( lastIntX, int( y ), C );
		}
		if ( int( X1 ) > lastIntX )
		{
			PutPixel( int( X1 ), int( Y1 ), C );
		}
	}
}

void Graphics::FillBox( const Rect & Box, unsigned Color )
{
	const auto left = max( -Box.left, 0 );
	const auto top = max( -Box.top, 0 );
	const auto right = min( m_screenWidth - 1 - Box.left, Box.GetWidth() );
	const auto bottom = min( m_screenHeight - 1 - Box.top, Box.GetHeight() );

	for ( int y = top; y < bottom; ++y )
	{
		DrawLine( left, y, right, y, Color );
	}
}

void Graphics::FillCircle( int X, int Y, int Radius, unsigned Color )
{
	if ( X > 0 && X - Radius < m_screenWidth &&
		 Y > 0 && Y - Radius < m_screenHeight )
	{
		int a = 0;
	}
	const auto _2Rad = Radius << 1;
	const auto _left = X - Radius;
	const auto _top = Y - Radius;

	const auto left = max( -_left, 0 ) - Radius;
	const auto top = max( -_top, 0 ) - Radius;
	const auto right = min( m_screenWidth - 1 - _left,   _2Rad ) - Radius;
	const auto bottom = min( m_screenHeight - 1 - _top, _2Rad ) - Radius;

	const auto sqRad = Radius * Radius;
	for ( int y = top; y < bottom; ++y )
	{
		for ( int x = left; x < right; ++x )
		{
			const auto sqDist = ( ( x*x ) + ( y*y ) );
			if ( sqDist <= sqRad )
			{
				PutPixel( x + X, y + Y, Color );
			}
		}
	}
}

void Graphics::OutlineBox( const Rect & Box, unsigned Color )
{
	const auto bottom = Box.bottom - 1l;

	DrawLine( Box.left, Box.top, Box.right, Box.top, Color );
	DrawLine( Box.left, bottom, Box.right, bottom, Color );
	DrawLine( Box.left, Box.top, Box.left, bottom, Color );
	DrawLine( Box.right, Box.top, Box.right, bottom, Color );
}

void Graphics::OutlineCircle( int X, int Y, int Radius, unsigned Color )
{
	const auto sqRadius = Radius * Radius;
	
	for ( int y = -Radius; y < Radius; ++y )
	{
		for ( int x = -Radius; x < Radius; ++x )
		{
			const auto dist = ( x*x ) + ( y*y );

			if ( dist <= sqRadius && dist > sqRadius - 4)
			{
				PutPixel( x, y, Color );
			}
		}
	}
}

