#ifndef _OBJECTFACTORY_H_
#define _OBJECTFACTORY_H_

#include "StructLibrary.h"
#include "D3Object.h"
#include "TextureHandler.h"
#include "SimpleMath.h"
#include "StructLibrary.h"
#include "TextureObject.h"
#include <fstream>

#include <string>
#include <vector>
#include <sstream>

class ObjectFactory
{
public:
	ObjectFactory();
	virtual ~ObjectFactory();


	vector<D3Object*> CreateFromFile(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* fileName, FactoryObjectFormat objectFormat);
	void CreateFromFile(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* fileName, FactoryObjectFormat objectFormat, vector<D3Object*> &storeIn);
	bool CreateFromObj(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* fileName, vector<D3Object*> &storeIn, int invert);

	void ReadObjMaterial(string filename, vector<ObjMaterial>& storeIn);
	vector<ObjMaterial> ReadObjMaterial(string filename);
};

#endif