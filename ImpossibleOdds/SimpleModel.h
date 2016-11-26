#pragma once

#include <memory>

struct ID3D11Buffer;

class SimpleModel
{
public:
	// Struct for storing the vertices of the quad used for drawing the back buffer to
	struct VertexBuffer
	{
		float px, py, pz, tu, tv;
	};

	SimpleModel();
	SimpleModel( std::shared_ptr<VertexBuffer[]> &pVertices );
	~SimpleModel();

	unsigned GetVertexCount()const;
	unsigned GetIndexCount()const;
	unsigned GetStride()const;
	ID3D11Buffer *GetVertexBuffer()const;
	ID3D11Buffer *GetIndexBuffer()const;

private:
	std::shared_ptr<VertexBuffer> m_pVertices;
	ID3D11Buffer *m_pVertexBuffer, *m_pIndexBuffer;
};

