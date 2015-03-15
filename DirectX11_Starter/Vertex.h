#pragma once

#include <DirectXMath.h>

using namespace DirectX;

// Convenience macro for releasing a COM object
#define ReleaseMacro(x) { if(x){ x->Release(); x = 0; } }

// Macro for popping up a text box based
// on a failed HRESULT and then quitting (only in debug builds)
#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)												\
{															\
	HRESULT hr = (x);										\
if (FAILED(hr))											\
{														\
	DXTrace(__FILEW__, (DWORD)__LINE__, hr, L#x, true);	\
	PostQuitMessage(0);									\
}														\
}
#endif
#else
#ifndef HR
#define HR(x) (x)
#endif
#endif

// Vertex struct for triangles
struct Vertex
{
	XMFLOAT3 Position;
	XMFLOAT2 UV;
	XMFLOAT3 Normal;
};

// Struct to match vertex shader's constant buffer
// You update one of these locally, then push it to the corresponding
// constant buffer on the device when it needs to be updated
struct VertexShaderConstantBufferLayout
{
	XMFLOAT4X4 world;
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
}; 

struct PixelShaderConstantBufferLayout
{
	XMFLOAT4 light1Color;
	XMFLOAT3 light1Direction;
	float padding1;
	XMFLOAT4 light2Color;
	XMFLOAT3 light2Direction;
	float padding2;
};