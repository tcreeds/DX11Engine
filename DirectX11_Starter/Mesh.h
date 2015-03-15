#pragma once

#include "Vertex.h"
#include "DirectXGame.h"
#include <vector>

using namespace DirectX;

class Mesh
{
public:
	Mesh();
	~Mesh();
	void SetData(Vertex* vertices, int numVertices, UINT* indices, int numIndices);
	void SetBuffers(ID3D11Buffer* vb, ID3D11Buffer* ib);
	ID3D11Buffer* const* getVertexBuffer();
	ID3D11Buffer* getIndexBuffer();
	int getIndexCount();

private:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	Vertex* vertices;
	int numVertices;
	UINT* indices;
	int numIndices;

};

