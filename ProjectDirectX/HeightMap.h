#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H
#include "D3Object.h"
class HeightMap: public D3Object
{
private:
	TextureObject* heightModel;
public:
	HeightMap();
	~HeightMap();
	bool LoadHeightMap(char *);
	void NormalizeHeightMap();
	void ShutDownHM();
	bool CreateFlatMesh();
	bool DisplaceVertices();


};

#endif