#include "GameObject.h"

GameObject::GameObject()
{
	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&transform, XMMatrixTranspose(W));
	velocity = XMFLOAT3(rand() % 5 -2.5f, rand() % 5 - 2.5f, 0);
	time = 1.57f;
}

GameObject::~GameObject()
{
}

void GameObject::Update(float dt){
	time += dt;
	Translate(sin(time) / 500.0f, 0, velocity.z * dt);
}

ID3D11Buffer* const* GameObject::getMeshVertexArray(){
	return mMesh->getVertexBuffer();
}

ID3D11Buffer* GameObject::getMeshIndexArray(){
	return mMesh->getIndexBuffer();
}

int GameObject::getMeshIndexCount(){
	return mMesh->getIndexCount();
}

XMFLOAT4X4 GameObject::getTransform(){
	return transform;
}

void GameObject::SetMesh(Mesh* mesh){
	this->mMesh = mesh;
}

void GameObject::SetMaterial(Material* material){
	this->mMaterial = material;
}

Material* GameObject::GetMaterial(){
	return this->mMaterial;
}

void GameObject::Translate(float x, float y, float z){
	XMMATRIX mat = XMMatrixTranspose(XMLoadFloat4x4(&transform));
	XMMATRIX trans = XMMatrixTranslation(x, y, z);
	XMMATRIX result = mat * trans;
	XMStoreFloat4x4(&transform, XMMatrixTranspose(result));
}

void GameObject::Rotate(float x, float y, float z){
	XMMATRIX mat = XMMatrixTranspose(XMLoadFloat4x4(&transform));
	XMMATRIX rot = XMMatrixRotationRollPitchYaw(x, y, z);
	XMMATRIX result = mat * rot;
	XMStoreFloat4x4(&transform, XMMatrixTranspose(result));
}

void GameObject::Rotate(XMFLOAT3 axis, float angle){
	XMMATRIX mat = XMMatrixTranspose(XMLoadFloat4x4(&transform));
	XMVECTOR vAxis = XMLoadFloat3(&axis);
	XMMATRIX rot = XMMatrixRotationAxis(vAxis, angle);
	XMMATRIX result = mat * rot;
	XMStoreFloat4x4(&transform, XMMatrixTranspose(result));
}

void GameObject::Scale(float x, float y, float z){
	XMMATRIX mat = XMMatrixTranspose(XMLoadFloat4x4(&transform));
	XMMATRIX scale = XMMatrixScaling(x, y, z);
	XMMATRIX result = mat * scale;
	XMStoreFloat4x4(&transform, XMMatrixTranspose(result));
}

void GameObject::Scale(float size){
	XMMATRIX mat = XMMatrixTranspose(XMLoadFloat4x4(&transform));
	XMMATRIX scale = XMMatrixScaling(size, size, size);
	XMMATRIX result = mat * scale;
	XMStoreFloat4x4(&transform, XMMatrixTranspose(result));
}
