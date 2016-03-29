#ifndef _FRUSTRUM_H_
#define _FRUSTRUM_H_

#include "StructLibrary.h"
using namespace DirectX::SimpleMath;
class Frustrum
{
private:	//Variables
	Plane m_planes[6];	//The planes extracted from the camera / projection matrix
public:
	Frustrum();
	Frustrum(const Frustrum& original);
	virtual ~Frustrum();

	void Initialize();

	bool GenerateFrustrum(Matrix view, Matrix proj, float screenDepth);

	bool TestAgainstPoint(Vector3* pos);
	bool TestAgainstCube(Vector3* pos, float length);
	bool TestAgainstRectangle(Vector3* pos, Vector3* sideDelta);
	bool TestAgainstSphere(Vector3* pos, float radius);
	bool TestAgainstRectangle(Vector2* middle, Vector2* sideDelta);

private:	//Functions

};

#endif