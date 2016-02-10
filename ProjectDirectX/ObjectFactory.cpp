#include "ObjectFactory.h"



ObjectFactory::ObjectFactory()
{
}


ObjectFactory::~ObjectFactory()
{
}

bool ObjectFactory::CreateObject(D3Object * storeIn, char * fileName, FactoryObjectFormat objectFormat)
{
	bool result = false;
	switch (objectFormat)
	{
	case OBJ:
		result = this->ReadOBJ(storeIn, fileName);
		break;
	default:
		break;
	}
	return result;
}

bool ObjectFactory::ReadOBJ(D3Object * storeIn, char * fileName)
{
	ifstream fileIn;
	string special = "", line = "", line2 = "";
	istringstream inputString;
	vector<Vector3> vertices;
	vector<Vector3> normals;
	vector<Vector2> UV;
	vector<VertexModel> vertexData;
	Vector3 vtx = { 0, 0, 0 }, vn = { 0, 0, 0 };
	Vector2 vt = { 0, 0 };

	fileIn.open(fileName, ios::in);
	if (!fileIn.is_open())
	{
		return false;
	}

	while (std::getline(fileIn, line2))
	{
		inputString.str(line2);
		if (line2.substr(0, 2) == "v ")
		{
			// Vertex Position
			inputString >> special >> vtx.x >> vtx.y >> vtx.z;
			vertices.push_back(vtx);
		}
		else if (line2.substr(0, 2) == "vt")
		{
			// Vertex UV
			inputString >> special >> vt.x >> vt.y;
			UV.push_back(vt);
		}
		else if (line2.substr(0, 2) == "vn")
		{
			// Vertex Normal
			inputString >> special >> vn.x >> vn.y >> vn.z;
			normals.push_back(vn);
		}
		else if (line2.substr(0, 2) == "g ")
		{
			//Group name
		}
		else if (line2.substr(0, 7) == "mtllib ")
		{
			//Material name
		}
		else if (line2.substr(0, 2) == "f ")
		{
			//Vertex Normal Indices in format f v1/vt1/vn1
			char temp[512];
			strncpy(temp, line2.c_str(), sizeof(temp));
			temp[sizeof(temp) - 1] = 0;
			char* pos = strstr(temp, "f ");
			struct IndexStruct { int v; int vt; int vn; } faceIndices[3];
			sscanf(pos, "%s %i/%i/%i %i/%i/%i %i/%i/%i\n", &special, &faceIndices[0].v, &faceIndices[0].vt, &faceIndices[0].vn,
				&faceIndices[1].v, &faceIndices[1].vt, &faceIndices[1].vn,
				&faceIndices[2].v, &faceIndices[2].vt, &faceIndices[2].vn);
			//VertexModel tempModelData = {vertices[faceIndices[0].v - 1], UV[faceIndices[0].v], normals[faceIndices[0].vn]};
			for (int i = 0; i < 3; i++)
				vertexData.push_back({ vertices[faceIndices[i].v - 1], UV[faceIndices[i].v - 1], normals[faceIndices[i].vn - 1] });
		}
	}
	fileIn.close();



	return true;
}
