#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "SimpleMath.h"
#include "StructLibrary.h"

using namespace DirectX::SimpleMath;
class Camera
{
private:
	Vector3 m_position;
	Vector3 m_rotation;
	Vector3 m_up;
	Matrix m_viewMatrix;
public:
	Camera();
	Camera(const Camera &original);
	~Camera();

	void SetPosition(float x, float y, float z);
	void SetPosition(Vector3 position);
	void SetRotation(float x, float y, float z);
	void SetRotation(Vector3 rotation);
	void SetUp(float x, float y, float z);
	void SetUp(Vector3 upVector);

	Vector3 GetPosition();
	Vector3 GetRotation();
	Vector3 GetUpVector();

	void Render();

	Matrix GetViewMatrix();
	void GetViewMatrix(Matrix& storeIn);
	void GetViewMatrix(WVPBufferStruct& storeIn);
};

#endif