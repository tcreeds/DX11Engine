#pragma once

#include <DirectXMath.h>
#include "DirectXGame.h"
#include "WICTextureLoader.h"
#include "Vertex.h"
#include "MeshFactory.h"
#include "GameObject.h"
#include "Camera.h"
#include "Material.h"

// Include run-time memory checking in debug builds
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

// For DirectX Math
using namespace DirectX;

struct DirectionalLight{
	XMFLOAT4 AmbientColor;
	XMFLOAT4 DiffuseColor;
	XMFLOAT3 Direction;
};

// Demo class which extends the base DirectXGame class
class MyDemoGame : public DirectXGame
{
public:
	MyDemoGame(HINSTANCE hInstance);
	~MyDemoGame();

	// Overrides for base level methods
	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene(); 

	// For handing mouse input
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

private:
	// Initialization for our "game" demo
	void CreateGeometryBuffers();
	void LoadShadersAndInputLayout();
	void InitializeCameraMatrices();
	void Draw(GameObject* obj);
	GameObject* CreateGameObject(const char* name);

private:

	// Our basic shaders for this example
	ID3D11PixelShader* pixelShader;
	ID3D11VertexShader* vertexShader;

	// A few more odds and ends we'll need
	ID3D11InputLayout* inputLayout;
	ID3D11Buffer* vsConstantBuffer;
	ID3D11Buffer* lightConstantBuffer;
	VertexShaderConstantBufferLayout dataToSendToVSConstantBuffer;
	PixelShaderConstantBufferLayout dataToSendToLightConstantBuffer;
	ID3D11ShaderResourceView* textureSRV;
	ID3D11SamplerState* samplerState;

	// The matrices to go from model space
	// to screen space
	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projectionMatrix;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;
	bool mouseDown;

	Camera* mainCamera;

	MeshFactory* meshFactory;
	GameObject* triangle;
	GameObject* square;
	GameObject* parallelogram;
	GameObject* entities[10];
	int entityCount;

	DirectionalLight light1;
	DirectionalLight light2;
};