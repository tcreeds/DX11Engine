#pragma once
#include <Windows.h>
#include <DirectXMath.h>
#include "Transform.h"

using namespace DirectX;

class Camera
{
public:
	Camera();
	~Camera();
	void Update(float dt);
	XMFLOAT4X4 getViewMatrix();
	void FollowTarget(Transform* target);
	void Rotate(float x, float y);
	enum CameraState{ FIRSTPERSON, THIRDPERSON, SCRIPTED };

private:
	Transform* target;
	XMFLOAT3 position;
	XMFLOAT3 direction;
	XMFLOAT3 forward;
	XMFLOAT3 up;
	XMFLOAT4X4 viewMatrix;
	float rotationX;
	float rotationY;
	CameraState cameraState;
	XMFLOAT3 distance;

};


