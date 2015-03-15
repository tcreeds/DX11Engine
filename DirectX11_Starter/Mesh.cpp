#include "Mesh.h"
#include <fstream>
#include <vector>

Mesh::Mesh()
{

}

Mesh::~Mesh()
{
	ReleaseMacro(this->vertexBuffer);
	ReleaseMacro(this->indexBuffer);
}

void Mesh::SetData(Vertex* vertices, int numVertices, UINT* indices, int numIndices){
	this->vertices = vertices;
	this->numVertices = numVertices;
	this->indices = indices;
	this->numIndices = numIndices;
}

void Mesh::SetBuffers(ID3D11Buffer* vb, ID3D11Buffer* ib){
	this->vertexBuffer = vb;
	this->indexBuffer = ib;
}

ID3D11Buffer* const* Mesh::getVertexBuffer(){
	return &vertexBuffer;
}

ID3D11Buffer* Mesh::getIndexBuffer(){
	return indexBuffer;
}

int Mesh::getIndexCount(){
	return numIndices;
}

