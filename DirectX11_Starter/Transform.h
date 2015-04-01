#pragma once
#include "Vertex.h"

class Transform
{
public:
	Transform();
	~Transform();
	void Translate(float x, float y, float z);
	void RotateEuler(float x, float y, float z);
	void RotateAxisAngle(XMFLOAT3 axis, float angle);
	void Scale(float x, float y, float z);
	void Scale(float size);
	void SetPosition(XMFLOAT3 pos);
	void SetPosition(float x, float y, float z);
	void SetRotation(XMFLOAT3 rot);
	void SetRotation(float x, float y, float z);
	void MoveForward(float amount);
	void Strafe(float amount);
	XMFLOAT3* GetPosition();
	XMFLOAT3* GetRotation();
	XMFLOAT4X4* GetMatrix();
	void UpdateMatrix();
	void UpdateVectors();	
	XMFLOAT3 forward;
	XMFLOAT3 right;
	XMFLOAT3 up;

private:
	XMFLOAT4X4 matrix;
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 scale;
};

