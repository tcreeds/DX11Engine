// ----------------------------------------------------------------------------
//  A few notes on project settings
//
//  - The project is set to use the UNICODE character set
//    - This was changed in Project Properties > Config Properties > General > Character Set
//    - This basically adds a "#define UNICODE" to the project
//
//  - The include directories were automagically correct, since the DirectX 
//    headers and libs are part of the windows SDK
//    - For instance, $(WindowsSDK_IncludePath) is set as a project include 
//      path by default.  That's where the DirectX headers are located.
//
//  - Two libraries had to be manually added to the Linker Input Dependencies
//    - d3d11.lib
//    - d3dcompiler.lib
//    - This was changed in Project Properties > Config Properties > Linker > Input > Additional Dependencies
//
//  - The Working Directory was changed to match the actual .exe's 
//    output directory, since we need to load the compiled shader files at run time
//    - This was changed in Project Properties > Config Properties > Debugging > Working Directory
//
// ----------------------------------------------------------------------------

#include <Windows.h>
#include <d3dcompiler.h>
#include "MyDemoGame.h"
#include <iostream>

#pragma region Win32 Entry Point (WinMain)

using namespace DirectX;

// Win32 Entry Point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	// Make the game, initialize and run
	MyDemoGame game(hInstance);
	
	// If we can't initialize, we can't run
	if( !game.Init() )
		return 0;
	
	// All set to run the game
	return game.Run();
}

#pragma endregion

#pragma region Constructor / Destructor

MyDemoGame::MyDemoGame(HINSTANCE hInstance) : DirectXGame(hInstance)
{
	// Set up our custom caption and window size
	windowCaption = L"Demo DX11 Game";
	windowWidth = 800;
	windowHeight = 600;
}

MyDemoGame::~MyDemoGame()
{
	delete meshFactory;
	delete triangle;
	delete square;
	delete parallelogram;
	delete mainCamera;
	for (int i = 0; i < entityCount; i++)
		delete entities[i];

	// Release all of the D3D stuff that's still hanging out
	ReleaseMacro(vertexShader);
	ReleaseMacro(pixelShader);
	ReleaseMacro(vsConstantBuffer);
	ReleaseMacro(inputLayout);
}

#pragma endregion

#pragma region Initialization

// Initializes the base class (including the window and D3D),
// sets up our geometry and loads the shaders (among other things)
bool MyDemoGame::Init()
{
	// Make sure DirectX initializes properly
	if( !DirectXGame::Init() )
		return false;


	mainCamera = new Camera();
	// Create the necessary DirectX buffers to draw something
	CreateGeometryBuffers();

	// Load pixel & vertex shaders, and then create an input layout
	LoadShadersAndInputLayout();

	// Set up camera-related matrices
	InitializeCameraMatrices();

	
	light1.AmbientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 1);
	light1.DiffuseColor = XMFLOAT4(0.5f, 0, 0, 1);
	light1.Direction = XMFLOAT3(0, -1, 0);

	light2.AmbientColor = XMFLOAT4(0, 0, 0, 1);
	light2.DiffuseColor = XMFLOAT4(0, 0, 0.5f, 1);
	light2.Direction = XMFLOAT3(-1, 0, 0);

	// Successfully initialized
	return true;
}

// Creates the vertex and index buffers for a single triangle
void MyDemoGame::CreateGeometryBuffers()
{

	entityCount = 0;
	meshFactory = new MeshFactory();
	meshFactory->init(device);
	meshFactory->loadOBJ("models/cube.obj", "Cube");
	meshFactory->loadOBJ("models/torus.obj", "Torus");
	meshFactory->loadOBJ("models/helix.obj", "Helix");
	meshFactory->loadOBJ("models/sphere.obj", "Sphere");
	meshFactory->loadOBJ("models/box.obj", "Box");

	/*GameObject* cube = new GameObject();
	cube->SetMesh(meshFactory->createMesh("Cube"));
	//cube->Scale(1);
	//cube->Translate(0.75f, 0.5f, 0);

	GameObject* torus = new GameObject();
	torus->SetMesh(meshFactory->createMesh("Torus"));
	//torus->Scale(1);
	//torus->Translate(-0.75f, 0.5f, 0);

	GameObject* helix = new GameObject();
	helix->SetMesh(meshFactory->createMesh("Helix"));

	GameObject* sphere = new GameObject();
	sphere->SetMesh(meshFactory->createMesh("Sphere"));
	//sphere->Scale(2);
	//sphere->Translate(0, -0.5f, 0);*/

	GameObject* box = new GameObject();
	box->SetMesh(meshFactory->createMesh("Box"));	

	//entities[entityCount++] = cube;
	//entities[entityCount++] = torus;
	//entities[entityCount++] = sphere;
	//entities[entityCount++] = helix;
	entities[entityCount++] = box;
	mainCamera->FollowTarget(entities[0]->transform);

	GameObject* obj = CreateGameObject("Helix");
	obj->transform->Translate(5, 0, 0);
	GameObject* obj2 = CreateGameObject("Torus");
	obj2->transform->Translate(-5, 0, 0);
	
}

GameObject* MyDemoGame::CreateGameObject(const char* name){
	GameObject* obj = new GameObject();
	obj->SetMesh(meshFactory->createMesh(name));
	entities[entityCount++] = obj;
	return obj;
}

// Loads shaders from compiled shader object (.cso) files, and uses the
// vertex shader to create an input layout which is needed when sending
// vertex data to the device
void MyDemoGame::LoadShadersAndInputLayout()
{
	// Set up the vertex layout description
	// This has to match the vertex input layout in the vertex shader
	// We can't set up the input layout yet since we need the actual vert shader
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// Load Vertex Shader --------------------------------------
	ID3DBlob* vsBlob;
	D3DReadFileToBlob(L"VertexShader.cso", &vsBlob);

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
	cBufferDesc.ByteWidth           = sizeof(dataToSendToVSConstantBuffer);
	cBufferDesc.Usage				= D3D11_USAGE_DEFAULT;
	cBufferDesc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	cBufferDesc.CPUAccessFlags		= 0;
	cBufferDesc.MiscFlags			= 0;
	cBufferDesc.StructureByteStride = 0;
	HR(device->CreateBuffer(
		&cBufferDesc,
		NULL,
		&vsConstantBuffer));

	D3D11_BUFFER_DESC lBufferDesc;
	lBufferDesc.ByteWidth			= sizeof(dataToSendToLightConstantBuffer);
	lBufferDesc.Usage				= D3D11_USAGE_DEFAULT;
	lBufferDesc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	lBufferDesc.CPUAccessFlags		= 0;
	lBufferDesc.MiscFlags			= 0;
	lBufferDesc.StructureByteStride = 0;
	HR(device->CreateBuffer(
		&lBufferDesc,
		NULL,
		&lightConstantBuffer));

	CreateWICTextureFromFile(device, deviceContext, L"textures/lavaland.png", 0, &textureSRV);
	D3D11_SAMPLER_DESC sdesc;
	ZeroMemory(&sdesc, sizeof(sdesc));
	sdesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	sdesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	sdesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	sdesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	sdesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&sdesc, &samplerState);

	Material* mat = new Material(vertexShader, pixelShader, textureSRV, samplerState);

	//Material* mat = new Material(L"VertexShader.cso", L"PixelShader.cso", device);
	for (int i = 0; i < entityCount; i++)
		entities[i]->SetMaterial(mat);

}

// Initializes the matrices necessary to represent our 3D camera
void MyDemoGame::InitializeCameraMatrices()
{
	// Create the View matrix
	// In an actual game, update this when the camera moves (every frame)
	XMVECTOR position = XMVectorSet(0, 0, -5, 0);
	XMVECTOR target = XMVectorSet(0, 0, 0, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V = XMMatrixLookAtLH(position, target, up); // View matrix creation:
                                                         // Looks at a "target" from
	                                                     // a particular "position"
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V));

	// Create the Projection matrix
	// This should match the window's aspect ratio, and also update anytime
	// the window resizes (which is already happening in OnResize() below)
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		AspectRatio(),				// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P));
}

#pragma endregion

#pragma region Window Resizing

// Handles resizing the window and updating our projection matrix to match
void MyDemoGame::OnResize()
{
	// Handle base-level DX resize stuff
	DirectXGame::OnResize();

	// Update our projection matrix since the window size changed
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,
		AspectRatio(),
		0.1f,
		100.0f);
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P));
}
#pragma endregion

#pragma region Game Loop

// Update your game state
void MyDemoGame::UpdateScene(float dt)
{
	mainCamera->Update(dt);


	if (GetAsyncKeyState(VK_LEFT) & 0x8000){
		entities[0]->transform->RotateEuler(0, 0.001f, 0);
	}
	else if (GetAsyncKeyState(VK_RIGHT) & 0x8000){
		entities[0]->transform->RotateEuler(0, -0.001f, 0);
	}
	else if (GetAsyncKeyState(VK_UP) & 0x8000){
		entities[0]->transform->RotateEuler(0.001f, 0, 0);
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000){
		entities[0]->transform->RotateEuler(-0.001f, 0, 0);
	}

	if (GetAsyncKeyState(0x57) & 0x8000){
		entities[0]->transform->MoveForward(0.01f);
	}
	if (GetAsyncKeyState(0x41) & 0x8000){
		entities[0]->transform->Strafe(-0.01f);
	}
	if (GetAsyncKeyState(0x53) & 0x8000){
		entities[0]->transform->MoveForward(-0.01f);
	}
	if (GetAsyncKeyState(0x44) & 0x8000){
		entities[0]->transform->Strafe(0.01f);
	}
	/*
	// Take input, update game logic, etc.
	for (int i = 0; i < entityCount; i++)
		entities[i]->Update(dt);
	*/
}

// Clear the screen, redraw everything, present
void MyDemoGame::DrawScene()
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = {0.4f, 0.6f, 0.75f, 0.0f};

	// Clear the buffer (erases what's on the screen)
	//  - Do this once per frame
	//  - At the beginning (before drawing anything)
	deviceContext->ClearRenderTargetView(renderTargetView, color);
	deviceContext->ClearDepthStencilView(
		depthStencilView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	dataToSendToLightConstantBuffer.light1Color = light1.DiffuseColor;
	dataToSendToLightConstantBuffer.light1Direction = light1.Direction;
	dataToSendToLightConstantBuffer.light2Color = light2.DiffuseColor;
	dataToSendToLightConstantBuffer.light2Direction = light2.Direction;

	//updates lighting constant buffer
	deviceContext->UpdateSubresource(
		lightConstantBuffer,
		0,
		NULL,
		&dataToSendToLightConstantBuffer,
		0,
		0);

	deviceContext->PSSetConstantBuffers(
		1,	// Corresponds to the constant buffer's register in the vertex shader
		1,
		&lightConstantBuffer);



	for (int i = 0; i < entityCount; i++)
		Draw(entities[i]);

	// Present the buffer
	//  - Puts the stuff on the screen
	//  - Do this EXACTLY once per frame
	//  - Always at the end of the frame
	HR(swapChain->Present(0, 0));
}

#pragma endregion

#pragma region Mouse Input

// These methods don't do much currently, but can be used for mouse-related input

void MyDemoGame::OnMouseDown(WPARAM btnState, int x, int y)
{
	mouseDown = true;
	prevMousePos.x = x;
	prevMousePos.y = y;

	SetCapture(hMainWnd);
}

void MyDemoGame::OnMouseUp(WPARAM btnState, int x, int y)
{
	mouseDown = false;
	ReleaseCapture();
}

void MyDemoGame::OnMouseMove(WPARAM btnState, int x, int y)
{

	if (mouseDown){
		/*float dy = (y - prevMousePos.y) / 70.0f;
		float dx = (x - prevMousePos.x) / 100.0f;
		
		entities[0]->Rotate(0, dx, 0);*/
		mainCamera->Rotate((y - prevMousePos.y) / 70.0f, (x - prevMousePos.x) / 70.0f);
	}
		

	prevMousePos.x = x;
	prevMousePos.y = y;
}
#pragma endregion

#pragma region Drawing

void MyDemoGame::Draw(GameObject* obj){

	Material* mat = obj->GetMaterial();
	// Copy CPU-side data to a single CPU-side structure
	//  - Allows us to send the data to the GPU buffer in one step
	//  - Do this PER OBJECT, before drawing it
	//mat->SetMatrices(&obj->getTransform(), &mainCamera->getViewMatrix(), &projectionMatrix);
	dataToSendToVSConstantBuffer.world = *(obj->transform->GetMatrix());
	dataToSendToVSConstantBuffer.view = mainCamera->getViewMatrix();
	dataToSendToVSConstantBuffer.projection = projectionMatrix;
	

	// Set up the input assembler
	//  - These technically don't need to be set every frame, unless you're changing the
	//    input layout (different kinds of vertices) or the topology (different primitives)
	//    between draws
	deviceContext->IASetInputLayout(inputLayout);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set the current vertex and pixel shaders
	//  - These don't need to be set every frame YET
	//  - Once you have multiple shaders, you will need to change these
	//    between drawing objects that will use different shaders
	deviceContext->VSSetShader(mat->GetVertexShader(), NULL, 0);
	deviceContext->PSSetShader(mat->GetPixelShader(), NULL, 0);
	deviceContext->PSSetShaderResources(0, 1, mat->GetTexture());
	deviceContext->PSSetSamplers(0, 1, mat->GetSampler());

	// Update the GPU-side constant buffer with our single CPU-side structure
	//  - Faster than setting individual sub-variables multiple times
	//  - Do this PER OBJECT, before drawing it
	deviceContext->UpdateSubresource(
		vsConstantBuffer,
		0,
		NULL,
		&dataToSendToVSConstantBuffer,
		0,
		0);

	// Set the constant buffer to be used by the Vertex Shader
	//  - This should be done PER OBJECT you intend to draw, as each object
	//    will probably have different data to send to the shader (matrices
	//    in this case)
	deviceContext->VSSetConstantBuffers(
		0,	// Corresponds to the constant buffer's register in the vertex shader
		1,
		&vsConstantBuffer);

	

	// Set buffers in the input assembler
	//  - This should be done PER OBJECT you intend to draw, as each object could
	//    potentially have different geometry (and therefore different buffers!)
	//  - You must have both a vertex and index buffer set to draw
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, obj->getMeshVertexArray(), &stride, &offset);
	deviceContext->IASetIndexBuffer(obj->getMeshIndexArray(), DXGI_FORMAT_R32_UINT, 0);


	// Finally do the actual drawing
	//  - This should be done PER OBJECT you index to draw
	//  - This will use all of the currently set DirectX stuff (shaders, buffers, etc)
	deviceContext->DrawIndexed(
		obj->getMeshIndexCount(),	// The number of indices we're using in this draw
		0,
		0);
}

#pragma endregion