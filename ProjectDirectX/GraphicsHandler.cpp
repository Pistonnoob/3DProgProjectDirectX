#include "GraphicsHandler.h"


GraphicsHandler::GraphicsHandler()
{
	m_Direct3D = nullptr;
	m_Camera = nullptr;
	m_Model = nullptr;
	m_TextureShader = nullptr;
	rotation = 0.0f;
	//m_shaderHandler = nullptr;
}

GraphicsHandler::GraphicsHandler(const GraphicsHandler &other)
{
}


GraphicsHandler::~GraphicsHandler()
{
}

bool GraphicsHandler::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;


	// Create the Direct3D object.
	m_Direct3D = new D3DClass;
	if (!m_Direct3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new Camera();
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);

	// Create the model object.
	m_Model = new D3Object();
	if (!m_Model)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "box.obj", "stone01.tga", FactoryObjectFormat::OBJ_RH);
	//result = m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "cube.txt", "stone01.tga", FactoryObjectFormat::TXT);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the TextureShader object.
	m_TextureShader = new TextureHandler();
	if (!this->m_TextureShader)
	{
		return false;
	}

	// Initialize the shaderHandler object.
	result = this->m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the TextureHandler object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsHandler::ShutDown()
{
	// Release the color shader object.
	if (this->m_TextureShader)
	{
		this->m_TextureShader->Shutdown();
		delete this->m_TextureShader;
		this->m_TextureShader = nullptr;
	}

	// Release the model object.
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	//Release the Direct3D object
	if (m_Direct3D)
	{
		m_Direct3D->ShutDown();
		delete m_Direct3D;
		m_Direct3D = nullptr;
	}
	return;
}

bool GraphicsHandler::Frame()
{
	bool result = true;
	this->rotation += 3.1415f * 0.01f;
	if (this->rotation > 720.0f)
	{
		this->rotation -= 360.0f;
	}
	//Render the graphics scene.
	result = this->Render();
	if (!result)
	{
		return false;
	}

	return result;
}

bool GraphicsHandler::Render()
{
	Matrix viewMatrix, projectionMatrix, worldMatrix;
	bool result = false;


	// Clear the buffers to begin the scene.
	this->m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	this->m_Direct3D->GetWorldMatrix(worldMatrix);
	this->m_Camera->GetViewMatrix(viewMatrix);
	this->m_Direct3D->GetProjectionMatrix(projectionMatrix);

	worldMatrix = DirectX::XMMatrixRotationAxis(SimpleMath::Vector4(0, 1, 0, 0), rotation);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(this->m_Direct3D->GetDeviceContext());

	// Render the model using the texture shader.
	result = this->m_TextureShader->Render(this->m_Direct3D->GetDeviceContext(), this->m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture());
	if (!result)
	{
		return false;
	}

	//Present the rendered scene to the screen.
	m_Direct3D->EndScene();
	return true;
}