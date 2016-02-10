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
	bool CreateObject(D3Object* storeIn, char* fileName, FactoryObjectFormat objectFormat);
	bool ReadOBJ(D3Object* storeIn, char* fileName);
};

#endif