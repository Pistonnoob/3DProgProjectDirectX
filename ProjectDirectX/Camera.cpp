#include "Camera.h"



Camera::Camera()
{
	m_position = Vector3(0.0f, 0.0f, 0.0f);
	m_rotation = Vector3(0.0f, 0.0f, 0.0f);
	m_up = Vector3(0.0f, 1.0f, 0.0f);
	m_viewMatrix = DirectX::XMMatrixIdentity();
}

Camera::Camera(const Camera & original)
{
	m_position = original.m_position;
	m_rotation = original.m_rotation;
	m_viewMatrix = original.m_viewMatrix;
}


Camera::~Camera()
{
}

void Camera::SetPosition(float x, float y, float z)
{
	this->m_position = Vector3(x, y, z);
	return;
}

void Camera::SetPosition(Vector3 position)
{
	this->m_position = position;
	return;
}

void Camera::SetRotation(float x, float y, float z)
{
	this->m_rotation = Vector3(x, y, z);
	return;
}

void Camera::SetRotation(Vector3 rotation)
{
	this->m_rotation = rotation;
	return;
}

void Camera::SetUp(float x, float y, float z)
{
	this->m_up = Vector3(x, y, z);
	return;
}

void Camera::SetUp(Vector3 upVector)
{
	this->m_up = upVector;
	return;
}

Vector3 Camera::GetPosition()
{
	return this->m_position;
}

Vector3 Camera::GetRotation()
{
	return this->m_rotation;
}

Vector3 Camera::GetUpVector()
{
	return this->m_up;
}

void Camera::Render()	//Builds and updates the view-matrix
{
	Vector4 up(0, 1.0f, 0, 0.0f), position(0, 0, 0, 1), lookAt(0, 0, 1, 1);
	float yaw = 0.0f, pitch = 0.0f, roll = 0.0f;
	Matrix rotationMatrix;
	//check in how the vector is rotated around its look-at. Normally (0, 1, 0), for standing upright.
	up = Vector4(m_up.x, m_up.y, m_up.z, 0.0f);
	//Setup the possition of the camera in world-space.
	position = Vector4(m_position.x, m_position.y, m_position.z, 1.0f);
	//Set where the camera is looking at by default.
	lookAt = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
	//Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = this->m_rotation.x * DEGREES_TO_RADIANS;
	yaw = this->m_rotation.y * DEGREES_TO_RADIANS;
	roll = this->m_rotation.z * DEGREES_TO_RADIANS;

	//Create the rotation matrix
	rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	//Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin
	lookAt = DirectX::XMVector3TransformCoord(lookAt, rotationMatrix);
	up = DirectX::XMVector3TransformCoord(up, rotationMatrix);
	
	//Translate the rotated camera position to the location of the viewer
	lookAt = position + lookAt;
	//Create the view matrix from our updated vectors
	this->m_viewMatrix = DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(position.x, position.y, position.z, 1), DirectX::XMVectorSet(lookAt.x, lookAt.y, lookAt.z, 1), DirectX::XMVectorSet(up.x, up.y, up.z, 1));
	return;
}

void Camera::GetViewMatrix(Matrix & storeIn)
{
	storeIn = this->m_viewMatrix;
	return;
}

void Camera::GetViewMatrix(WVPBufferStruct & storeIn)
{
	//this->GetViewMatrix(storeIn.view);
	storeIn.view = this->m_viewMatrix;
	return;
}
