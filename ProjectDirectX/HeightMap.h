#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H
#include "D3Object.h"
class HeightMap: public D3Object
{
private:
	HeightMapPos* m_heightMap;
	int m_terrainWidth, m_terrainHeight;
public:
	HeightMap();
	~HeightMap();
	bool LoadHeightMap(char *);
	void NormalizeHeightMap();
	void ShutDownHM();
	bool InitializeBuffersHeightMap(ID3D11Device *device);
					
	/*bool CreateFlatMesh();
	bool DisplaceVertices();*/


};

#endif