#include "Material.h"
#include <d3dcompiler.h>

using namespace DirectX;

Material::Material(const WCHAR* vertexShaderFile, const WCHAR* pixelShaderFile, ID3D11Device* device){

	// Set up the vertex layout description
	// This has to match the vertex input layout in the vertex shader
	// We can't set up the input layout yet since we need the actual vert shader
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// Load Vertex Shader --------------------------------------
	ID3DBlob* vsBlob;
	D3DReadFileToBlob(vertexShaderFile, &vsBlob);

	// Create the shader on the device
	HR(device->CreateVertexShader(
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		NULL,
		&vertexShader));

	// Before cleaning up the data, create the input layout
	HR(device->CreateInputLayout(
		vertexDesc,
		ARRAYSIZE(vertexDesc),
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		&inputLayout));

	// Clean up
	ReleaseMacro(vsBlob);

	// Load Pixel Shader ---------------------------------------
	ID3DBlob* psBlob;
	D3DReadFileToBlob(L"PixelShader.cso", &psBlob);

	// Create the shader on the device
	HR(device->CreatePixelShader(
		psBlob->GetBufferPointer(),
		psBlob->GetBufferSize(),
		NULL,
		&pixelShader));

	// Clean up
	ReleaseMacro(psBlob);

	// Constant buffers ----------------------------------------
	D3D11_BUFFER_DESC cBufferDesc;
	cBufferDesc.ByteWidth = sizeof(bufferData);
	cBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cBufferDesc.CPUAccessFlags = 0;
	cBufferDesc.MiscFlags = 0;
	cBufferDesc.StructureByteStride = 0;
	HR(device->CreateBuffer(
		&cBufferDesc,
		NULL,
		&vsConstantBuffer));



}

Material::Material(ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader)
	:pixelShader(pixelShader), vertexShader(vertexShader)
{
}

Material::Material(ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader, ID3D11ShaderResourceView* textureSRV, ID3D11SamplerState* samplerState)
	:pixelShader(pixelShader), vertexShader(vertexShader), textureSRV(textureSRV), samplerState(samplerState)
{
}


Material::~Material()
{
}

ID3D11VertexShader* Material::GetVertexShader(){
	return this->vertexShader;
}

ID3D11PixelShader* Material::GetPixelShader(){
	return this->pixelShader;
}

VertexShaderConstantBufferLayout* Material::GetConstantBufferData(){
	return bufferData;
}

ID3D11InputLayout* Material::GetInputLayout(){
	return inputLayout;
}

ID3D11Buffer* Material::GetConstantBuffer(){
	return vsConstantBuffer;
}

ID3D11ShaderResourceView** Material::GetTexture(){
	return &textureSRV;
}

ID3D11SamplerState** Material::GetSampler(){
	return &samplerState;
}

void Material::SetMatrices(XMFLOAT4X4* world, XMFLOAT4X4* view, XMFLOAT4X4* projection){
	bufferData->world = *(world);
	bufferData->view = *(view);
	bufferData->projection = *(projection);

}
