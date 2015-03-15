#pragma once

#include "Mesh.h"
#include "Vertex.h"
#include "MeshData.h"
#include <fstream>
#include <vector>

class MeshFactory
{
public:
	MeshFactory();
	~MeshFactory();
	void init(ID3D11Device* device);
	void loadOBJ(std::string filepath, std::string meshName);
	Mesh* createMesh(std::string name);

private:
	ID3D11Buffer* createVertexBuffer(Vertex* vertices, int numVertices);
	ID3D11Buffer* createIndexBuffer(UINT* indices, int numIndices);
	ID3D11Device* device;
	MeshData* meshes[10];
	int numMeshes;
};

