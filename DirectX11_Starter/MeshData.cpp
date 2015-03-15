#include "MeshData.h"


MeshData::MeshData()
{
}

MeshData::MeshData(std::string name, std::vector<Vertex> vertices, int numVertices, std::vector<UINT> indices, int numIndices){
	this->name = name;
	this->vertices = vertices;
	this->numVertices = numVertices;
	this->indices = indices;
	this->numIndices = numIndices;
}

std::string MeshData::GetName(){
	return name;
}

Vertex* MeshData::GetVertices(){
	return vertices.data();
}

unsigned int MeshData::GetNumVertices(){
	return numVertices;
}

UINT* MeshData::GetIndices(){
	return indices.data();
}

unsigned int MeshData::GetNumIndices(){
	return numIndices;
}


MeshData::~MeshData()
{

}
