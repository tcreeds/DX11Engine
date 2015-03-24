#include "Camera.h"
#include <iostream>


Camera::Camera()
{
	position = XMFLOAT3(0, 0, -10);
	forward = XMFLOAT3(0, 0, 1);
	up = XMFLOAT3(0, 1, 0);
	XMStoreFloat4x4(&viewMatrix, XMMatrixIdentity());
	rotationX = 0.0f;
	rotationY = 0.0f;
	distance = XMFLOAT3(0, 5, -15);
	
}


Camera::~Camera()
{
}

void Camera::Update(float dt){

	/*if (GetAsyncKeyState(VK_UP) & 0x8000){
		position.z += direction.z / 100;
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000){
		position.z -= direction.z / 100;
	}*/

	if (cameraState == THIRDPERSON){
		float rotation = target->GetRotation()->y;
		XMFLOAT3* pos = target->GetPosition();
		//position = XMFLOAT3(pos->x + distance.x, pos->y + distance.y, pos->z + distance.z);
		position = XMFLOAT3(pos->x + sin(rotation) * distance.z, pos->y + distance.y, pos->z + cos(rotation) * distance.z);
		XMMATRIX trans = XMMatrixLookAtLH(XMLoadFloat3(&position), XMLoadFloat3(target->GetPosition()), XMLoadFloat3(&up));
		XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(trans));

	}
	else if (cameraState == FIRSTPERSON){

		XMMATRIX mat = XMMatrixRotationRollPitchYaw(rotationX, rotationY, 0.0f);
		XMVECTOR dir = XMVector3Transform(XMLoadFloat3(&forward), mat);
		XMStoreFloat3(&direction, dir);
		XMMATRIX trans = XMMatrixLookToLH(XMLoadFloat3(&position), dir, XMLoadFloat3(&up));
		XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(trans));

	}

	

	
	
	//std::cerr << "camera positon: " << position.x << " " << position.y << " " << position.z << std::endl;

}

XMFLOAT4X4 Camera::getViewMatrix(){
	return viewMatrix;
}

void Camera::Rotate(float x, float y){

	this->rotationX = fmod(this->rotationX + x, XM_2PI);
	this->rotationY = fmod(this->rotationY + y, XM_2PI);

}

void Camera::FollowTarget(Transform* target){
	this->cameraState = THIRDPERSON;
	this->target = target;

}

