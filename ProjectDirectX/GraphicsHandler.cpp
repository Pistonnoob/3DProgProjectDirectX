#include "GraphicsHandler.h"


GraphicsHandler::GraphicsHandler()
{
	m_Direct3D = nullptr;
	m_Camera = nullptr;
	m_TextureShader = nullptr;
	m_Light = { Vector4(0.0f, 0.0f, 0.0f, 0.0f), Vector4(0, 0, 0, 0), Vector4(0, 0, 0, 0)};
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
	bool result = false;


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

	this->LoadScene(hwnd);

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
	/*if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}*/

	// Release the models within the model list.
	while(!m_Models.empty())
	{
		D3Object* temp = NULL;
		temp = m_Models.back();
		m_Models.pop_back();
		delete temp;
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

bool GraphicsHandler::Frame(int fps, float frameTime, InputHandler* inputObj)
{
	this->UpdateInput(inputObj, frameTime / 1000);
	bool result = true;
	this->rotation += 3.1415f * 0.01f;
	if (this->rotation > 720.0f)
	{
		this->rotation = -360.0f;
	}

	return result;
}

bool GraphicsHandler::UpdateInput(InputHandler* inputObj, float dT)
{
#pragma region
	float verticalConstant = 2, horizontalConstant = 2;
	//Change dT based on vertical vs horizontal movement
	float origDT = dT;
	dT *= verticalConstant;	//Update the vertical movement
	if (inputObj->IsKeyPressed(DIK_W))
	{
		Vector4 tempPos = Vector4(D_FRONT.x, D_FRONT.y, D_FRONT.z, 0);
		//Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
		Vector3 rotation = m_Camera->GetRotation();
		rotation *= DEGREES_TO_RADIANS;
		//Create the rotation matrix
		Matrix rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
		tempPos = DirectX::XMVector4Transform(tempPos, rotationMatrix);
		tempPos *= dT;
		m_Camera->SetPosition(m_Camera->GetPosition() + Vector3(tempPos));
	}
	if (inputObj->IsKeyPressed(DIK_S))
	{
		Vector4 tempPos = Vector4(D_BACK.x, D_BACK.y, D_BACK.z, 0);
		//Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
		Vector3 rotation = m_Camera->GetRotation();
		rotation *= DEGREES_TO_RADIANS;
		//Create the rotation matrix
		Matrix rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
		tempPos = DirectX::XMVector4Transform(tempPos, rotationMatrix);
		tempPos *= dT;
		m_Camera->SetPosition(m_Camera->GetPosition() + Vector3(tempPos));
	}

	dT = origDT;	//Change the vertical movement back

	dT *= horizontalConstant;	//Update the horizontal movement
	if (inputObj->IsKeyPressed(DIK_A))
	{
		Vector4 tempPos = Vector4(D_LEFT.x, D_LEFT.y, D_LEFT.z, 0);
		//Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
		Vector3 rotation = m_Camera->GetRotation();
		rotation *= DEGREES_TO_RADIANS;
		//Create the rotation matrix
		Matrix rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
		tempPos = DirectX::XMVector4Transform(tempPos, rotationMatrix);
		tempPos *= dT;
		m_Camera->SetPosition(m_Camera->GetPosition() + Vector3(tempPos));
	}
	if (inputObj->IsKeyPressed(DIK_D))
	{
		Vector4 tempPos = Vector4(D_RIGHT.x, D_RIGHT.y, D_RIGHT.z, 0);
		//Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
		Vector3 rotation = m_Camera->GetRotation();
		rotation *= DEGREES_TO_RADIANS;
		//Create the rotation matrix
		Matrix rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
		tempPos = DirectX::XMVector4Transform(tempPos, rotationMatrix);
		tempPos *= dT;
		m_Camera->SetPosition(m_Camera->GetPosition() + Vector3(tempPos));
	}
	dT = origDT;	//Change the horizontal movement back
	//Move upwards
	if (inputObj->IsKeyPressed(DIK_SPACE))
	{
		int up = 1;
		if (inputObj->IsKeyPressed(DIK_LSHIFT))
		{
			up *= -1;
		}
		Vector4 tempPos = Vector4(D_UP.x, D_UP.y, D_UP.z, 0);
		tempPos *= (float)up;
		//Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
		Vector3 rotation = m_Camera->GetRotation();
		rotation *= DEGREES_TO_RADIANS;
		//Create the rotation matrix
		Matrix rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
		tempPos = DirectX::XMVector4Transform(tempPos, rotationMatrix);
		tempPos *= dT;
		m_Camera->SetPosition(m_Camera->GetPosition() + Vector3(tempPos));
	}

	//Allow for resetting the camera back to its origin
	if (inputObj->IsKeyPressed(DIK_R))
	{
		m_Camera->SetPosition(ORIG);
		m_Camera->SetRotation(Vector3(0, 0, 0));
	}
	
#pragma endregion keyboard
#pragma region

	this->m_Camera->SetRotation(this->m_Camera->GetRotation() + Vector3{ inputObj->GetMouseDelta().y / 4, inputObj->GetMouseDelta().x / 4, 0 });

#pragma endregion mouse
	return true;
}

bool GraphicsHandler::LoadScene(HWND hwnd)
{
	bool result = false;

	// Create the camera object.
	m_Camera = new Camera();
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);

	//Create the lights.
	m_Light = {Vector4(255, 255, 255, 1.0f), Vector4(0, 0, -2, 1), Vector4(0, 0, 1, 0)};

	// Create the model objects.
	//Test factory creating model objects.
	ObjectFactory factory;
	result = factory.CreateFromFile(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "box.obj", FactoryObjectFormat::OBJ_RH, this->m_Models);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	result = factory.CreateFromFile(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "sphere1.obj", FactoryObjectFormat::OBJ_RH, this->m_Models);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	if (m_Models.size() > 1)
	{
		Matrix tempWorld;
		Matrix temp2World;
		m_Models[1]->GetWorldMatrix(tempWorld);
		m_Models[1]->ApplyMatrix(XMMatrixTranslation(0.0f, 5.0f, 0.0f));
		m_Models[1]->GetWorldMatrix(temp2World);
		if (temp2World != tempWorld)
		{
			int thereWasNoAppliedMatrix = 0;
		}
	}
	/*result = factory.CreateFromFile(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "box.obj", FactoryObjectFormat::OBJ_RH, this->m_Models);
	int size = m_Models.size();
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}*/

	return true;
}

bool GraphicsHandler::Render()
{
	// Clear the buffers to begin the scene.
	this->m_Direct3D->BeginScene(0.1f, 0.1f, 0.1f, 1.0f);
	Matrix viewMatrix, projectionMatrix, worldMatrix;
	bool result = false;

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the view, and projection matrices from the camera and d3d objects.
	this->m_Camera->GetViewMatrix(viewMatrix);
	this->m_Direct3D->GetProjectionMatrix(projectionMatrix);

	for (std::vector<D3Object*>::iterator model = this->m_Models.begin(); model != this->m_Models.end(); model++)
	{
		//Do the logic uniqueue to every model / object

		//Get the world matrix from the model / object and rotate it for visibility
		(*model)->GetWorldMatrix(worldMatrix);
		worldMatrix *= XMMatrixRotationAxis(SimpleMath::Vector4(0, 1, 0, 0), rotation);

		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		(*model)->Render(this->m_Direct3D->GetDeviceContext());

		// Render the model using the texture shader.
		result = this->m_TextureShader->Render(this->m_Direct3D->GetDeviceContext(), (*model)->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Light, (*model)->GetTexture());
		if (!result)
		{
			return false;
		}
	}

	//Present the rendered scene to the screen.
	m_Direct3D->EndScene();
	return true;
}