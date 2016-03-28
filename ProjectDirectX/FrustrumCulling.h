#ifndef _FRUSTRUMCULLING_H_
#define _FRUSTRUMCULLING_H_

#include "StructLibrary.h"
using namespace DirectX::SimpleMath;
class FrustrumCulling
{
private:	//Variables
	Plane m_planes[6];	//The planes extracted from the camera / projection matrix
public:
	FrustrumCulling();
	FrustrumCulling(const FrustrumCulling& original);
	virtual ~FrustrumCulling();

	void Initialize();

	bool GenerateFrustrum(Matrix view, Matrix proj, float screenDepth);

	bool TestAgainstPoint(Vector3* pos);
	bool TestAgainstCube(Vector3* pos, float length);
	bool TestAgainstRectangle(Vector3* pos, Vector3* sideLengths);
	bool TestAgainstSphere(Vector3* pos, float radius);

private:	//Functions

};

#endif