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
	Vector3 up(0, 0, 0), position(0, 0, 0), lookAt(0, 0, 0);
	float yaw = 0.0f, pitch = 0.0f, roll = 0.0f;
	Matrix rotationMatrix;
	//check in how the vector is rotated around its look-at. Normally (0, 1, 0), for standing upright.
	up = this->m_up;
	//Setup the possition of the camera in world-space.
	position = m_position;
	//Set where the camera is looking at by default.
	lookAt = Vector3(0.0f, 0.0f, 1.0f);
	//Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = this->m_rotation.x * 0.0174532925f;
	yaw = this->m_rotation.y * 0.0174532925f;
	roll = this->m_rotation.z * 0.0174532925f;

	//Create the rotation matrix
	rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	//Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin
	lookAt = DirectX::XMVector2TransformCoord(lookAt, rotationMatrix);
	up = DirectX::XMVector2TransformCoord(up, rotationMatrix);
	//DirectX::XMMATRIX
	//Translate the rotated cmaera position to the locaiton of the viewer
	lookAt = position + lookAt;
	//Create the view matrix from our updated vectors
	this->m_viewMatrix = DirectX::XMMatrixLookAtLH(position, lookAt, up);
	return;
}

void Camera::GetViewMatrix(Matrix & storeIn)
{
	storeIn = this->m_viewMatrix;
	return;
}

void Camera::GetViewMatrix(MatrixBufferStruct & storeIn)
{
	//this->GetViewMatrix(storeIn.view);
	storeIn.view = this->m_viewMatrix;
	return;
}
