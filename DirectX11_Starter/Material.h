#pragma once
#include <Windows.h>
#include <d3d11.h>
#include "DirectXGame.h"
#include "Vertex.h"

using namespace DirectX;

class Material
{
public:
	Material(const WCHAR* vertexShaderPath, const WCHAR* pixelShaderPath, ID3D11Device* device);
	Material(ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader);
	Material(ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader, ID3D11ShaderResourceView* textureSRV, ID3D11SamplerState* samplerState);
	~Material();
	ID3D11VertexShader* GetVertexShader();
	ID3D11PixelShader* GetPixelShader();
	VertexShaderConstantBufferLayout* GetConstantBufferData();
	ID3D11InputLayout* GetInputLayout();
	ID3D11Buffer* GetConstantBuffer();
	ID3D11ShaderResourceView** GetTexture();
	ID3D11SamplerState** GetSampler();
	void SetMatrices(XMFLOAT4X4* world, XMFLOAT4X4* view, XMFLOAT4X4* perspective);

private: 
	ID3D11InputLayout* inputLayout;
	ID3D11Buffer* vsConstantBuffer;
	VertexShaderConstantBufferLayout* bufferData;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11ShaderResourceView* textureSRV;
	ID3D11SamplerState* samplerState;
};

