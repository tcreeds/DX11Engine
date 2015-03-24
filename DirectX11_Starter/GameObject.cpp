#include "GameObject.h"

GameObject::GameObject()
{
	this->transform = new Transform();
	velocity = XMFLOAT3(rand() % 5 -2.5f, rand() % 5 - 2.5f, 0);
	time = 1.57f;
}

GameObject::~GameObject()
{
}

void GameObject::Update(float dt){
	time += dt;
	transform->Translate(sin(time) / 500.0f, 0, velocity.z * dt);
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

void GameObject::SetMesh(Mesh* mesh){
	this->mMesh = mesh;
}

void GameObject::SetMaterial(Material* material){
	this->mMaterial = material;
}

Material* GameObject::GetMaterial(){
	return this->mMaterial;
}


