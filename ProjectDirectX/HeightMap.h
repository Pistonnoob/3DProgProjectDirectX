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
	bool CreateFlatMesh();
	bool DisplaceVertices();


};

#endif