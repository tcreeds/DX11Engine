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
	
}


Camera::~Camera()
{
}

void Camera::Update(float dt){
	
	//rotationY += 0.01f;

	if (GetAsyncKeyState(VK_UP) & 0x8000){
		position.z += direction.z / 100;
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000){
		position.z -= direction.z / 100;
	}

	

	XMMATRIX mat = XMMatrixRotationRollPitchYaw(rotationX, rotationY, 0.0f);
	XMVECTOR dir = XMVector3Transform( XMLoadFloat3(&forward), mat );
	XMStoreFloat3(&direction, dir);
	XMMATRIX trans = XMMatrixLookToLH(XMLoadFloat3(&position), dir, XMLoadFloat3(&up));
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(trans));
	
	//std::cerr << "camera positon: " << position.x << " " << position.y << " " << position.z << std::endl;

}

XMFLOAT4X4 Camera::getViewMatrix(){
	return viewMatrix;
}

void Camera::Rotate(float x, float y){

	this->rotationX = fmod(this->rotationX + x, XM_2PI);
	this->rotationY = fmod(this->rotationY + y, XM_2PI);

}
