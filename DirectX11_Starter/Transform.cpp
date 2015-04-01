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
	position = XMFLOAT3(position.x + x, position.y + y, position.z + z);
	UpdateMatrix();
}

void Transform::RotateEuler(float x, float y, float z){
	rotation = XMFLOAT3(rotation.x + x, rotation.y + y, rotation.z + z);
	UpdateMatrix();
}

void Transform::RotateAxisAngle(XMFLOAT3 axis, float angle){
	XMMATRIX mat = XMMatrixTranspose(XMLoadFloat4x4(&matrix));
	XMVECTOR vAxis = XMLoadFloat3(&axis);
	XMMATRIX rot = XMMatrixRotationAxis(vAxis, angle);
	XMMATRIX result = rot * mat;
	XMStoreFloat4x4(&matrix, XMMatrixTranspose(result));
}

void Transform::Scale(float x, float y, float z){
	scale = XMFLOAT3(scale.x * x, scale.y * y, scale.z * z);
	UpdateMatrix();
}

void Transform::Scale(float size){
	scale = XMFLOAT3(scale.x * size, scale.y * size, scale.z * size);
	UpdateMatrix();
}

void Transform::SetPosition(XMFLOAT3 pos){
	position.x = pos.x;
	position.y = pos.y;
	position.z = pos.z;
	UpdateMatrix();
}

void Transform::SetPosition(float x, float y, float z){
	position.x = x;
	position.y = y;
	position.z = z;
	UpdateMatrix();
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

void Transform::UpdateMatrix(){
	
	XMMATRIX mat = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	mat *= XMMatrixScaling(scale.x, scale.y, scale.z);
	mat *= XMMatrixTranslation(position.x, position.y, position.z);
	XMStoreFloat4x4(&matrix, XMMatrixTranspose(mat));
	UpdateVectors();

}

void Transform::UpdateVectors(){

	right =		XMFLOAT3(matrix._11, matrix._21, matrix._31);
	up =		XMFLOAT3(matrix._12, matrix._22, matrix._32);
	forward =	XMFLOAT3(matrix._13, matrix._23, matrix._33);

}
