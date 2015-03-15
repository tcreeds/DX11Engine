#pragma once
#include "DirectXGame.h"
#include <fstream>
#include <vector>
#include "Vertex.h"

class MeshData
{
public:
	MeshData();
	MeshData(std::string name, std::vector<Vertex> vertices, int numVertices, std::vector<UINT> indices, int numIndices);
	std::string GetName();
	Vertex* GetVertices();
	unsigned int GetNumVertices();
	UINT* GetIndices();
	unsigned int GetNumIndices();
	~MeshData();

private:
	std::string name;
	std::vector<Vertex> vertices;
	unsigned int numVertices;
	std::vector<UINT> indices;
	unsigned int numIndices;

};

