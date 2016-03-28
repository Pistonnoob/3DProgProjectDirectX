#include "Frustrum.h"



Frustrum::Frustrum()
{
}

Frustrum::Frustrum(const Frustrum & original)
{
}


Frustrum::~Frustrum()
{
}

void Frustrum::Initialize()
{
	for (int i = 0; i < ARRAYSIZE(m_planes); i++)
	{
		m_planes[i] = Plane(0.0f, 0.0f, 0.0f, 0.0f);
	}
}

bool Frustrum::GenerateFrustrum(Matrix view, Matrix proj, float screenDepth)
{
	float zMin = 0.0f, float r = 0.0f;
	Matrix m;
	//Calc the min z distance in the frustrum
	zMin = -proj._43 / proj._33;
	r = screenDepth / (screenDepth - zMin);
	proj._33 = r;
	proj._43 = -r * zMin;
	m = DirectX::XMMatrixMultiply(view, proj);

	//Calc near
	m_planes[0].x = m._14 + m._13;
	m_planes[0].y = m._24 + m._23;
	m_planes[0].z = m._34 + m._33;
	m_planes[0].w = m._44 + m._43;
	m_planes[0].Normalize();
	//Calc far
	m_planes[1].x = m._14 - m._13;
	m_planes[1].y = m._24 - m._23;
	m_planes[1].z = m._34 - m._33;
	m_planes[1].w = m._44 - m._43;
	m_planes[1].Normalize();
	//Calc left
	m_planes[2].x = m._14 + m._11;
	m_planes[2].y = m._24 + m._21;
	m_planes[2].z = m._34 + m._31;
	m_planes[2].w = m._44 + m._41;
	m_planes[2].Normalize();
	//Calc right
	m_planes[3].x = m._14 - m._11;
	m_planes[3].y = m._24 - m._21;
	m_planes[3].z = m._34 - m._31;
	m_planes[3].w = m._44 - m._41;
	m_planes[3].Normalize();
	//Calc top
	m_planes[4].x = m._14 - m._12;
	m_planes[4].y = m._24 - m._22;
	m_planes[4].z = m._34 - m._32;
	m_planes[4].w = m._44 - m._42;
	m_planes[4].Normalize();
	//Calc bottom
	m_planes[5].x = m._14 + m._12;
	m_planes[5].y = m._24 + m._22;
	m_planes[5].z = m._34 + m._32;
	m_planes[5].w = m._44 + m._42;
	m_planes[5].Normalize();

	return true;
}

bool Frustrum::TestAgainstPoint(Vector3 * pos)
{
	for (int i = 0; i < 6; i++)
	{
		if (m_planes[i].DotCoordinate(*pos) < 0.0f)
		{
			return false;
		}
	}
	return true;
}

bool Frustrum::TestAgainstCube(Vector3 * pos, float radius)
{
	return false;
	for (int i = 0; i < 6; i++)
	{
		if (m_planes[i].DotCoordinate(*pos + Vector3(radius, radius, radius)) >= 0.0f)
		{
			continue;
		}
		if (m_planes[i].DotCoordinate(*pos + Vector3(-radius, radius, radius)) >= 0.0f)
		{
			continue;
		}
		if (m_planes[i].DotCoordinate(*pos + Vector3(radius, -radius, radius)) >= 0.0f)
		{
			continue;
		}
		if (m_planes[i].DotCoordinate(*pos + Vector3(radius, radius, -radius)) >= 0.0f)
		{
			continue;
		}
		if (m_planes[i].DotCoordinate(*pos + Vector3(-radius, -radius, radius)) >= 0.0f)
		{
			continue;
		}
		if (m_planes[i].DotCoordinate(*pos + Vector3(-radius, radius, -radius)) >= 0.0f)
		{
			continue;
		}
		if (m_planes[i].DotCoordinate(*pos + Vector3(radius, -radius, -radius)) >= 0.0f)
		{
			continue;
		}
		if (m_planes[i].DotCoordinate(*pos + Vector3(-radius, -radius, -radius)) >= 0.0f)
		{
			continue;
		}
		return false;
	}

	return true;
}

bool Frustrum::TestAgainstRectangle(Vector3 * pos, Vector3 * sideLengths)
{
	for (int i = 0; i < 6; i++)
	{
		if (m_planes[i].DotCoordinate(*pos + *sideLengths) >= 0.0f)
		{
			continue;
		}
		if (m_planes[i].DotCoordinate(*pos + Vector3(-sideLengths->x, sideLengths->y, sideLengths->z)) >= 0.0f)
		{
			continue;
		}
		if (m_planes[i].DotCoordinate(*pos + Vector3(sideLengths->x, -sideLengths->y, sideLengths->z)) >= 0.0f)
		{
			continue;
		}
		if (m_planes[i].DotCoordinate(*pos + Vector3(sideLengths->x, sideLengths->y, -sideLengths->z)) >= 0.0f)
		{
			continue;
		}
		if (m_planes[i].DotCoordinate(*pos + Vector3(-sideLengths->x, -sideLengths->y, sideLengths->z)) >= 0.0f)
		{
			continue;
		}
		if (m_planes[i].DotCoordinate(*pos + Vector3(-sideLengths->x, sideLengths->y, -sideLengths->z)) >= 0.0f)
		{
			continue;
		}
		if (m_planes[i].DotCoordinate(*pos + Vector3(sideLengths->x, -sideLengths->y, -sideLengths->z)) >= 0.0f)
		{
			continue;
		}
		if (m_planes[i].DotCoordinate(*pos - *sideLengths) >= 0.0f)
		{
			continue;
		}
		return false;
	}
	return true;
}

bool Frustrum::TestAgainstSphere(Vector3 * pos, float radius)
{
	for (int i = 0; i < 6; i++)
	{
		if (m_planes[i].DotCoordinate(*pos) < -radius)
		{
			return false;
		}
	}
	return true;
}
