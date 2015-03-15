#pragma once
#include "Mesh.h"
#include "Material.h"


class GameObject
{
public:
	GameObject();
	virtual ~GameObject();
	void Update(float dt);
	ID3D11Buffer* const* getMeshVertexArray();
	ID3D11Buffer* getMeshIndexArray();
	int getMeshIndexCount();
	XMFLOAT4X4 getTransform();
	void SetMesh(Mesh* mesh);
	void SetMaterial(Material* material);
	Material* GetMaterial();
	void Translate(float x, float y, float z);
	void Rotate(float x, float y, float z);
	void Rotate(XMFLOAT3 axis, float angle);
	void Scale(float x, float y, float z);
	void Scale(float size);

private:
	Mesh* mMesh;
	Material* mMaterial;
	XMFLOAT4X4 transform;
	XMFLOAT3 velocity;
	float time;
};

