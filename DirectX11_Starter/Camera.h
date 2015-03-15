#pragma once
#include <Windows.h>
#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
public:
	Camera();
	~Camera();
	void Update(float dt);
	XMFLOAT4X4 getViewMatrix();
	void Rotate(float x, float y);

private:

	XMFLOAT3 position;
	XMFLOAT3 direction;
	XMFLOAT3 forward;
	XMFLOAT3 up;
	XMFLOAT4X4 viewMatrix;
	float rotationX;
	float rotationY;
};

