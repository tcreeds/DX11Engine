#include "Transform.h"


Transform::Transform()
{
	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&matrix, XMMatrixTranspose(W));
	position = XMFLOAT3(0, 0, 0);
	rotation = XMFLOAT3(0, 0, 0);
	scale = XMFLOAT3(1, 1, 1);
	forward = XMFLOAT3(0, 0, 1);
	right = XMFLOAT3(1, 0, 0);
	up = XMFLOAT3(0, 1, 0);
}


Transform::~Transform()
{
}

void Transform::Translate(float x, float y, float z){
	XMMATRIX mat = XMMatrixTranspose(XMLoadFloat4x4(&matrix));
	position.x += x;
	position.y += y;
	position.z += z;
	XMMATRIX trans = XMMatrixTranslation(x, y, z);
	XMMATRIX result = mat * trans;
	XMStoreFloat4x4(&matrix, XMMatrixTranspose(result));
}

void Transform::RotateEuler(float x, float y, float z){
	rotation.x += x;
	rotation.y += y;
	rotation.x += z;
	XMMATRIX mat = XMMatrixTranspose(XMLoadFloat4x4(&matrix));
	XMMATRIX rot = XMMatrixRotationRollPitchYaw(x, y, z);
	XMMATRIX result = mat * rot;
	XMStoreFloat4x4(&matrix, XMMatrixTranspose(result));
	UpdateVectors();
}

void Transform::RotateAxisAngle(XMFLOAT3 axis, float angle){
	XMMATRIX mat = XMMatrixTranspose(XMLoadFloat4x4(&matrix));
	XMVECTOR vAxis = XMLoadFloat3(&axis);
	XMMATRIX rot = XMMatrixRotationAxis(vAxis, angle);
	XMMATRIX result = rot * mat;
	XMStoreFloat4x4(&matrix, XMMatrixTranspose(result));
}

void Transform::Scale(float x, float y, float z){
	XMMATRIX mat = XMMatrixTranspose(XMLoadFloat4x4(&matrix));
	XMMATRIX scale = XMMatrixScaling(x, y, z);
	XMMATRIX result = mat * scale;
	XMStoreFloat4x4(&matrix, XMMatrixTranspose(result));
}

void Transform::Scale(float size){
	XMMATRIX mat = XMMatrixTranspose(XMLoadFloat4x4(&matrix));
	XMMATRIX scale = XMMatrixScaling(size, size, size);
	XMMATRIX result = mat * scale;
	XMStoreFloat4x4(&matrix, XMMatrixTranspose(result));
}

void Transform::SetPosition(XMFLOAT3 pos){
	position.x = pos.x;
	position.y = pos.y;
	position.z = pos.z;
}

void Transform::SetPosition(float x, float y, float z){
	position.x = x;
	position.y = y;
	position.z = z;
}

void Transform::SetRotation(XMFLOAT3 rot){

}

void Transform::MoveForward(float amount){
	Translate(forward.x * amount, forward.y * amount, forward.z * amount);
}

void Transform::Strafe(float amount){
	Translate(right.x * amount, right.y * amount, right.z * amount);
}

XMFLOAT3* Transform::GetPosition(){
	return &position;
}

XMFLOAT3* Transform::GetRotation(){
	return &rotation;
}

XMFLOAT4X4* Transform::GetMatrix(){
	return &matrix;
}

void Transform::UpdateVectors(){

	forward = XMFLOAT3(0, 0, 1);
	XMVECTOR f = XMVector3Transform(XMLoadFloat3(&forward), XMLoadFloat4x4(&matrix));
	XMStoreFloat3(&forward, f);

	right = XMFLOAT3(1, 0, 0);
	XMVECTOR r = XMVector3Transform(XMLoadFloat3(&right), XMLoadFloat4x4(&matrix));
	XMStoreFloat3(&right, r);

	XMVECTOR u = XMVector3Cross(f, r);	
}
