#pragma once
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"


class GameObject
{
public:
	GameObject();
	virtual ~GameObject();
	void Update(float dt);
	ID3D11Buffer* const* getMeshVertexArray();
	ID3D11Buffer* getMeshIndexArray();
	int getMeshIndexCount();
	void SetMesh(Mesh* mesh);
	void SetMaterial(Material* material);
	Material* GetMaterial();
	Transform* transform;

private:
	Mesh* mMesh;
	Material* mMaterial;
	//XMFLOAT4X4 transform;
	XMFLOAT3 velocity;
	float time;
};

