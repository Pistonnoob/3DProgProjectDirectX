#include "GraphicsHandler.h"


GraphicsHandler::GraphicsHandler()
{
	m_Direct3D = nullptr;
	m_Camera = nullptr;
	m_TextureShader = nullptr;
	m_FullScreenObject = nullptr;
	m_DeferredBuffers = nullptr;
	m_DeferredShader = nullptr;
	m_LightShader = nullptr;
	m_frustrum = nullptr;
	m_quadTree = nullptr;

	rotation = 0.0f;
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
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_FAR, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	this->LoadScene(hwnd);


	m_frustrum = new Frustrum();
	if (!m_frustrum)
	{
		return false;
	}
	m_frustrum->Initialize();

	m_quadTree = new QuadTree();
	if (!m_quadTree)
	{
		return false;
	}
	Vector2 min = Vector2(-500.0f, -500.0f);
	Vector2 max = Vector2(500.0f, 500.0f);
	this->m_quadTree->Initialize(min, max, 5);
	m_quadTree->DefineQuadTree(this->m_Models);

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

	m_FullScreenObject = new QuadModel();
	if (!m_FullScreenObject)
	{
		return false;
	}


	result = m_FullScreenObject->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the QuadModel object.", L"Error", MB_OK);
		return false;
	}

	m_DeferredBuffers = new DeferredBuffer();
	if (!m_DeferredBuffers)
	{
		return false;
	}

	result = m_DeferredBuffers->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_FAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Deferred Buffer object.", L"Error", MB_OK);
		return false;
	}

	m_DeferredShader = new DeferredHandler();
	if (!m_DeferredShader)
	{
		return false;
	}

	result = m_DeferredShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Deferred Handler object.", L"Error", MB_OK);
		return false;
	}

	m_LightShader = new LightShader();
	if (!m_LightShader)
	{
		return false;
	}

	result = m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Light Shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsHandler::ShutDown()
{
	//Release the Direct3D object
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = nullptr;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the models within the model list.
	/*while (!m_Models.empty())
	{
		D3Object* temp = NULL;
		temp = m_Models.back();
		m_Models.pop_back();
		temp->Shutdown();
		delete temp;
	}*/

	// Release the lights within the light list.
	while (!m_Lights.empty())
	{
		LightStruct* temp = NULL;
		temp = m_Lights.back();
		m_Lights.pop_back();
		delete temp;
	}

	// Release the color shader object.
	if (this->m_TextureShader != NULL)
	{
		this->m_TextureShader->Shutdown();
		delete this->m_TextureShader;
		this->m_TextureShader = nullptr;
	}

	// Release our precious post processing quad
	if (m_FullScreenObject != NULL)
	{
		this->m_FullScreenObject->Shutdown();
		delete this->m_FullScreenObject;
		this->m_FullScreenObject = NULL;
	}
	
	// Release our deferred data container
	if (m_DeferredBuffers != NULL)
	{
		this->m_DeferredBuffers->Shutdown();
		delete this->m_DeferredBuffers;
		this->m_DeferredBuffers = NULL;
	}

	if (this->m_DeferredShader != NULL)
	{
		this->m_DeferredShader->Shutdown();
		delete this->m_DeferredShader;
		this->m_DeferredShader = NULL;
	}

	if (this->m_LightShader != NULL)
	{
		this->m_LightShader->Shutdown();
		delete this->m_LightShader;
		this->m_LightShader = NULL;
	}

	if (this->m_frustrum != NULL)
	{
		delete this->m_frustrum;
		this->m_frustrum = NULL;
	}

	if (this->m_quadTree != NULL)
	{
		this->m_quadTree->ShutDown();
		delete this->m_quadTree;
		this->m_quadTree = NULL;
	}

	return;
}

bool GraphicsHandler::Frame(int fps, float frameTime, InputHandler* inputObj)
{
	this->UpdateInput(inputObj, frameTime / 1000);
	bool result = true;
	/*this->rotation += 360 * 0.005f * (frameTime / 1000);
	if (this->rotation > 720.0f)
	{
		this->rotation = -360.0f;
	}*/

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
	if (!inputObj->IsKeyPressed(DIK_C))
	{
		Vector3 resultRotation = Vector3{ inputObj->GetMouseDelta().y / 4, inputObj->GetMouseDelta().x / 4, 0 };
		Vector3 startRot = m_Camera->GetRotation();
		if ((startRot.x < CAMERA_Y_LOWER_BOUND && resultRotation.x < 0) || (startRot.x > CAMERA_Y_UPPER_BOUND && resultRotation.x > 0))
		{
			resultRotation.x = 0.0f;
		}
		resultRotation += startRot;
		m_Camera->SetRotation(resultRotation);
	}

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
	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();
	m_Camera->GenerateBaseViewMatrix();

	//Create the lights.
	LightStruct *Light2 = new LightStruct{ Vector4(50.0f, 50.0f, 50.0f, 1.0f) / 255.0f, Vector4(255.0f, 255.0f, 255.0f, 1.0f) / 255.0f, Vector4(255.0f, 255.0f, 255.0f, 1.0f) / 255.0f, Vector4(0.0f, 5.0f, -5.0f, 1.0f), Vector4(0.0f, 0.0f, 0.0f, 1.0f)};
	Light2->ambientColor.w = Light2->diffuseColor.w = Light2->specularColor.w = 1.0f;
	m_Lights.push_back(Light2);
	// Create the model objects.
	ObjectFactory factory;
	//int modelSize = 0;
	//for (int index = 0; index < 2; index++)
	//{
	//	result = factory.CreateFromFile(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "Ogre.obj", FactoryObjectFormat::OBJ_RH, this->m_Models);
	//	if (!result)
	//	{
	//		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
	//		return false;
	//	}
	//	if (index == 0)
	//		modelSize = m_Models.size();
	//	for (int modelIndex = index * modelSize; modelIndex < index * modelSize + modelSize; modelIndex++)
	//	{
	//		m_Models[modelIndex]->ApplyMatrix(XMMatrixTranslationFromVector(Vector3(0.0f, 0.0f, -20.0f)*(float)index));
	//		//m_Models[index]->ApplyMatrix(XMMatrixScaling(1, 1, -1));
	//	}

	//}

	result = factory.CreateFromFile(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "GrassPlane.obj", FactoryObjectFormat::OBJ_RH, this->m_Models);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	int modelIndex = m_Models.size();
	result = factory.CreateFromFile(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "cubes.obj", FactoryObjectFormat::OBJ_RH, this->m_Models);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	int modelDist = m_Models.size() - modelIndex;
	for (int i = modelIndex; i < m_Models.size(); i++)
	{
		m_Models[i]->ApplyMatrix(XMMatrixTranslationFromVector(Vector3(0.0f, 2.0f, 0.0f)));
	}


	/*result = factory.CreateFromFile(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "City.obj", FactoryObjectFormat::OBJ_RH, this->m_Models);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}*/
	

	//result = factory.CreateFromFile(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "Eggpod.obj", FactoryObjectFormat::OBJ_RH, this->m_Models);
	//if (!result)
	//{
	//	MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
	//	return false;
	//}
	//if (m_Models.size() > 0)
	//{
	//	m_Models[0]->ApplyMatrix(XMMatrixScaling(0.2f, 0.2f, 0.2f));
	//	m_Models[0]->ApplyMatrix(XMMatrixTranslation(2.2f, -5.0f, 4.0f));
	//	//m_Models[1]->ApplyMatrix(XMMatrixScaling(0.1f, 0.1f, 0.1f));
	//	//m_Models[1]->ApplyMatrix(XMMatrixTranslation(0.0f, -5.0f, 4.0f));
	//}
	//
	///*result = factory.CreateFromFile(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "quadBrick_Y_up.obj", FactoryObjectFormat::OBJ_RH, this->m_Models);
	//if (!result)
	//{
	//	MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
	//	return false;
	//}*/
	//int modelNr = 0;
	//for (std::vector<D3Object*>::iterator modelPtr = m_Models.begin(); modelPtr != m_Models.end(); modelPtr++)
	//{
	//	(*modelPtr)->ApplyMatrix(XMMatrixTranslationFromVector(Vector3(3.0f, 0.0f, 0.0f)*(float)modelNr));
	//	//(*modelPtr)->ApplyMatrix(XMMatrixScaling(1, 1, -1));
	//	modelNr++;
	//}
	return true;
}

bool GraphicsHandler::Render()
{

	Matrix baseViewMatrix, orthoMatrix, worldMatrix;
	bool result = false;

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	result = this->RenderToDeferred();
	if (!result)
	{
		return false;
	}

	// Clear the buffers to begin the scene.
	this->m_Direct3D->BeginScene(0.1f, 0.1f, 0.1f, 1.0f);

	//// Get the view, and projection matrices from the camera and d3d objects.
	this->m_Camera->GetBaseViewMatrix(baseViewMatrix);
	this->m_Direct3D->GetOrthoMatrix(orthoMatrix);
	worldMatrix = DirectX::XMMatrixIdentity();
	//Update the cameraPosition for the pixelshaders specular calculations
	Vector4 cameraPos = Vector4(m_Camera->GetPosition());
	//Turn off the z buffering
	this->m_Direct3D->TurnZBufferOff();

	//Now put the fullscreen Quads vertices and indices to be rendered
	this->m_FullScreenObject->Render(this->m_Direct3D->GetDeviceContext());
	WVPBufferStruct matrices = { worldMatrix, baseViewMatrix, orthoMatrix };
	for (std::vector<LightStruct*>::iterator light = m_Lights.begin(); light != m_Lights.end(); light++)
	{
		LightStructTemp tempLight = {Vector3((*light)->lightPos.x, (*light)->lightPos.y, (*light)->lightPos.z), 1.0f, Vector3((*light)->diffuseColor.x, (*light)->diffuseColor.y, (*light)->diffuseColor.z), 0.0f, cameraPos};
		//Do the light shading post processing on our quad
		//this->m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_FullScreenObject->GetIndexCount(), &matrices, m_DeferredBuffers->GetShaderResourceViews(), &tempLight);
		result = this->m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_FullScreenObject->GetIndexCount(), &matrices, m_DeferredBuffers->GetShaderResourceView(0), m_DeferredBuffers->GetShaderResourceView(1), m_DeferredBuffers->GetShaderResourceView(2), m_DeferredBuffers->GetShaderResourceView(3), m_DeferredBuffers->GetShaderResourceView(4), &tempLight);
		if (!result)
		{
			return false;
		}
	}

	//Return the z buffer to "ON" so the next 3d rendering can take place correctly
	m_Direct3D->TurnZBufferOn();

	m_Direct3D->EndScene();
	return true;
}

bool GraphicsHandler::RenderToDeferred()
{
	Matrix viewMatrix, projectionMatrix, worldMatrix;
	bool result = false;

	//Set the deferred buffers as the render target
	m_DeferredBuffers->SetRenderTargets(this->m_Direct3D->GetDeviceContext());
	//Clear the render buffers.
	m_DeferredBuffers->ClearRenderTargets(this->m_Direct3D->GetDeviceContext(), SimpleMath::Color(0.0f, 0.0f, 0.0f, 1.0f));

	// Get the view, and projection matrices from the camera and d3d objects.
	this->m_Camera->GetViewMatrix(viewMatrix);
	this->m_Direct3D->GetProjectionMatrix(projectionMatrix);

	this->m_frustrum->GenerateFrustrum(viewMatrix, projectionMatrix, SCREEN_FAR);

	std::vector<D3Object*> toRender;
	this->m_quadTree->GetObjectsInFrustrum(&toRender, this->m_frustrum);

	for (std::vector<D3Object*>::iterator model = toRender.begin(); model != toRender.end(); model++)
	{

		(*model)->GetWorldMatrix(worldMatrix);
		//Rotate the model
		worldMatrix = XMMatrixMultiply(XMMatrixRotationAxis(SimpleMath::Vector4(0, 1, 0, 0), rotation), worldMatrix);
		//Bind the vertices and indices to the pipeline
		(*model)->Render(m_Direct3D->GetDeviceContext());

		WVPBufferStruct matrices = { worldMatrix, viewMatrix, projectionMatrix };
		ObjMaterial objMaterial = (*model)->GetMaterial();
		PixelMaterial pMaterial = { Vector4(objMaterial.Ka.x, objMaterial.Ka.y, objMaterial.Ka.z, 0.0f), Vector4(objMaterial.Kd.x, objMaterial.Kd.y, objMaterial.Kd.z, 0.0f), Vector4(objMaterial.Ks.x, objMaterial.Ks.y, objMaterial.Ks.z, 0.0f), objMaterial.Ns, Vector3()};
		pMaterial.Ka = Vector4(objMaterial.Ka.x, objMaterial.Ka.y, objMaterial.Ka.z, 0.0f);
		pMaterial.Kd = Vector4(objMaterial.Kd.x, objMaterial.Kd.y, objMaterial.Kd.z, 0.0f);
		pMaterial.Ks = Vector4(objMaterial.Ks.x, objMaterial.Ks.y, objMaterial.Ks.z, 0.0f);
		pMaterial.Ns = objMaterial.Ns;
		pMaterial.padding = Vector3(0.0f, 0.0f, 0.0f);
		//Render the model using our brand new deferred renderer!
		m_DeferredShader->Render(m_Direct3D->GetDeviceContext(), (*model)->GetIndexCount(), &matrices, (*model)->GetTexture(), &pMaterial);
	}
	//toRender.clear();
	//Reset the render target to the back buffer
	m_Direct3D->SetBackBufferRenderTarget();
	//Reset the viewport
	m_Direct3D->ResetViewport();
	return true;
}

void GraphicsHandler::Click(int x, int y, int screenWidth, int screenHeight)
{
	Matrix P, V, W;
	m_Direct3D->GetProjectionMatrix(P);
	m_Camera->GetViewMatrix(V);
	V = V.Invert();
	float vx = 0, vy = 0, viewspaceZ = 1;
	vx = ((2 * x) / float(screenWidth) - 1) /*/ P(0, 0)*/;
	vx /= P(0, 0);
	vy = (-(2 * y) / float(screenHeight) + 1) /*/ P(1, 1)*/;
	vy /= P(1, 1);
	viewspaceZ = 1;
	Vector3 rayO(0.0f, 0.0f, 0.0f);
	Vector3 rayD(vx, vy, 1.0f);
	rayO = DirectX::XMVector3TransformCoord(rayO, V);
	rayD = DirectX::XMVector3TransformNormal(rayD, V);

	Vector3 dirfrac = Vector3(1.0f, 1.0f, 1.0f) / rayD;
	//Now we have our ray origin and direction.
	vector<Container*> possible;
	this->m_quadTree->GetObjectsInFrustrum(&possible, m_frustrum);
	for (std::vector<Container*>::const_iterator testBound = possible.begin(); testBound != possible.end(); testBound++)
	{
		float t = 0.0f;
		bool intersectionBox = true, intersectionModel = false;
		float distToModel = 0.0f;
		//For every model, check ray intersection against OBB
		//Get the Boundingbox
		BoundingVolume testVolume = (*testBound)->boundingVolume;
		//Extract the min and max
		Vector3 min = testVolume.middle - testVolume.sideDelta;
		Vector3 max = testVolume.middle + testVolume.sideDelta;
		float t1 = (min.x - rayO.x) * dirfrac.x, t2 = (max.x - rayO.x) * dirfrac.x;
		float t3 = (min.y - rayO.y) * dirfrac.y, t4 = (max.y - rayO.y) * dirfrac.y;
		float t5 = (min.z - rayO.z) * dirfrac.z, t6 = (max.z - rayO.z) * dirfrac.z;
		float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
		float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

		if (tmax < 0)
		{
			t = tmax;
			intersectionBox = false;
		}
		if (tmin > tmax)
		{
			t = tmax;
			intersectionBox = false;
		}

		//If there was a successfull intersection
		if (intersectionBox)
		{
			D3Object* object = (*testBound)->object;
			VertexModel* points = object->getVertedData();
			//Test intersection against contained model
			//Get the world matrice
			object->GetWorldMatrix(W);
			W = W.Invert();
			//Apply it on our ray
			rayO = DirectX::XMVector3TransformCoord(rayO, W);
			rayD = DirectX::XMVector3TransformNormal(rayD, W);
			//Get the vertices
			for (int i = 0; i < object->GetVertexCount() && !intersectionModel; i += 3)
			{
				//Test intersection against vertices
				float det = 0.0f, invDet = 0.0f;
				float dist = -1, u = 0.0f, v = 0.0f;
				//get the two edges
				Vector3 edge1 = points[i + 1].position - points[i].position,
						edge2 = points[i + 2].position - points[i].position;
				Vector3 pVec = rayD.Cross(edge2);
				Vector3 qVec(0.0f, 0.0f, 0.0f);
				det = edge1.Dot(pVec);
				if (det < -0.000000001f || det > 0.000000001f)
				{
					invDet = 1.0f / det;
					Vector3 tVec = rayO - points[i].position;
					u = tVec.Dot(pVec) * invDet;
					if (u >= 0.0f && u <= 1)
					{
						qVec = tVec.Cross(edge1);
						v = rayD.Dot(qVec);
						v *= invDet;
						if (v >= 0.0f && u + v <= 1.0f)
						{
							dist = edge2.Dot(qVec) * invDet;
						}
					}
				}
				if (dist >= 0.0f)
				{
					intersectionModel = true;
					distToModel = dist;
				}
			}
			if (intersectionModel)
			{
				bool success = true;
				ObjMaterial material = object->GetMaterial();
				material.Ns = 1.0f;
				object->SetMaterial(material);
			}
		}
	}

}













