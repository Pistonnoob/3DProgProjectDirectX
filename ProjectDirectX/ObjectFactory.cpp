#include "ObjectFactory.h"



ObjectFactory::ObjectFactory()
{
}


ObjectFactory::~ObjectFactory()
{
}

vector<D3Object*> ObjectFactory::CreateFromFile(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char * fileName, FactoryObjectFormat objectFormat)
{
	vector<D3Object*> storeIn;
	this->CreateFromFile(device, deviceContext, fileName, objectFormat, storeIn);
	return storeIn;
}

bool ObjectFactory::CreateFromFile(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char * fileName, FactoryObjectFormat objectFormat, vector<D3Object*> &storeIn)
{
	bool result = false;
	switch (objectFormat)
	{
	case OBJ:
	case OBJ_LH:
		result = this->CreateFromObj(device, deviceContext, fileName, storeIn, 1);
		break;
	case OBJ_RH:
		result = this->CreateFromObj(device, deviceContext, fileName, storeIn, -1);
		break;
	case TXT:
		break;
	default:
		break;
	}
	return result;
}

bool ObjectFactory::CreateFromObj(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char * fileName, vector<D3Object*>& storeIn, int invert)
{
	ifstream fileIn;
	string special = "", line = "", line2 = "";
	bool objectFinished = false;
	const unsigned int NAMELENGTH = 30;
	const unsigned int SPECIALCHARSIZE = 10;
	char specialChar[SPECIALCHARSIZE];
	istringstream inputString;
	vector<Vector3> vertices;
	vector<Vector3> normals;
	vector<Vector2> UV;
	char name[NAMELENGTH];
	char materialFile[NAMELENGTH];
	char objectMaterial[NAMELENGTH];
	vector<VertexModel> vertexData;
	vector<int> indexData;
	vector<ObjMaterial> materials;
	/*This should not be needed but may be used to make small test changes
	to the default material without causing a ripple effect on the system*/
	ObjMaterial missingMaterial = DEFAULT_MATERIAL;
	Vector3 vtx = { 0, 0, 0 }, vn = { 0, 0, 0 };
	Vector2 vt = { 0, 0 };

	char temp[512];

	fileIn.open(fileName, ios::in);
	if (!fileIn.is_open())
	{
		return false;
	}

	while (std::getline(fileIn, line2))
	{
		strncpy_s(temp, 512, line2.c_str(), sizeof(temp));
		temp[sizeof(temp) - 1] = 0;

		inputString.str(line2);
		if (line2.substr(0, 2) == "v ")
		{
			if (strcmp(specialChar, "f") == 0)
			{
				//Create a new 3DObject using the vectors
				D3Object* newObject = new D3Object();
				//Load the model data
				newObject->CreateFromData(vertexData);
				vertexData.clear();
				indexData.clear();
				//Initialize vertex and index buffers.
				newObject->InitializeBuffers(device);
				//Get the material name
				string textureName = "";
				//Initialize the material as "missing material"
				ObjMaterial localMaterial = missingMaterial;
				for (vector<ObjMaterial>::iterator mat = materials.begin(); mat != materials.end(); mat++)
				{
					if (strcmp((*mat).name,  objectMaterial) == 0)
					{
						localMaterial = (*mat);
					}
				}
				//Load the texture for this model
				newObject->LoadTexture(device, deviceContext, localMaterial.texture, localMaterial.textureFormat);
				newObject->SetMaterial(localMaterial);
				storeIn.push_back(newObject);
			}
			// Vertex Position
			sscanf_s(temp, "%s %f %f %f\n", specialChar, SPECIALCHARSIZE, &vtx.x, &vtx.y, &vtx.z);
			//vtx.z *= invert;
			//inputString >> special >> vtx.x >> vtx.y >> vtx.z;
			/*if (invert < 0)
				vtx.z = -vtx.z;*/
			vertices.push_back(vtx);
		}
		else if (line2.substr(0, 2) == "vt")
		{
			// Vertex UV
			sscanf_s(temp, "%s %f %f\n", specialChar, SPECIALCHARSIZE, &vt.x, &vt.y);
			if(invert < 0)
				vt.y = 1 - vt.y;
			//inputString >> special >> vt.x >> vt.y;
			UV.push_back(vt);
		}
		else if (line2.substr(0, 2) == "vn")
		{
			// Vertex Normal
			sscanf_s(temp, "%s %f %f %f\n", specialChar, SPECIALCHARSIZE, &vn.x, &vn.y, &vn.z);
			//inputString >> special >> vn.x >> vn.y >> vn.z;
			/*if (invert < 0)
				vn.z = -vn.z;*/
			normals.push_back(vn);
		}
		else if (line2.substr(0, 2) == "f ")
		{
			//Vertex Normal Indices in format f v1/vt1/vn1
			struct IndexStruct { int v; int vt; int vn; } faceIndices[3];
			if(invert > 0)
				sscanf_s(temp, "%s %i/%i/%i %i/%i/%i %i/%i/%i\n", specialChar, SPECIALCHARSIZE,
					&faceIndices[0].v, &faceIndices[0].vt, &faceIndices[0].vn,
					&faceIndices[1].v, &faceIndices[1].vt, &faceIndices[1].vn,
					&faceIndices[2].v, &faceIndices[2].vt, &faceIndices[2].vn);
			else
				sscanf_s(temp, "%s %i/%i/%i %i/%i/%i %i/%i/%i\n", specialChar, SPECIALCHARSIZE,
					&faceIndices[0].v, &faceIndices[0].vt, &faceIndices[0].vn,
					&faceIndices[2].v, &faceIndices[2].vt, &faceIndices[2].vn,
					&faceIndices[1].v, &faceIndices[1].vt, &faceIndices[1].vn);
			//VertexModel tempModelData = {vertices[&faceIndices[0].v - 1], UV[&faceIndices[0].v], normals[&faceIndices[0].vn]};
			for (int i = 0; i < 3; i++)
				vertexData.push_back({ vertices[faceIndices[i].v - 1], UV[faceIndices[i].vt - 1], normals[faceIndices[i].vn - 1] });

		}
		else if (line2.substr(0, 2) == "g ")
		{
			//Group name
			sscanf_s(temp, "%s %s\n", specialChar, SPECIALCHARSIZE, name, NAMELENGTH);
		}
		else if (line2.substr(0, 6) == "mtllib")
		{
			//Material File
			sscanf_s(temp, "%s %s\n", specialChar, SPECIALCHARSIZE, materialFile, NAMELENGTH);
			this->ReadObjMaterial(materialFile, materials);
		}
		else if (line2.substr(0, 6) == "usemtl")
		{
			//Material name for object / group
			sscanf_s(temp, "%s %s\n", specialChar, SPECIALCHARSIZE, objectMaterial, NAMELENGTH);
		}
	}
	fileIn.close();
	//Create a new 3DObject using the vectors
	D3Object* newObject = new D3Object();
	//Load the model data
	newObject->CreateFromData(vertexData);
	//Initialize vertex and index buffers.
	newObject->InitializeBuffers(device);
	//Get the material name
	string textureName = "";
	//Initialize the material as "missing material"
	ObjMaterial localMaterial = missingMaterial;
	for (vector<ObjMaterial>::iterator mat = materials.begin(); mat != materials.end(); mat++)
	{
		if (strcmp((*mat).name, objectMaterial) == 0)
		{
			localMaterial = (*mat);
		}
	}
	//Load the texture for this model
	newObject->LoadTexture(device, deviceContext, localMaterial.texture, localMaterial.textureFormat);
	newObject->SetMaterial(localMaterial);
	storeIn.push_back(newObject);

	return true;
}

bool ObjectFactory::CreateFromHeightMap(ID3D11Device * device, ID3D11DeviceContext * deviceContext, char * fileName, vector<D3Object*>& storeIn, int invert)
{
	return false;
}


void ObjectFactory::ReadObjMaterial(string filename, vector<ObjMaterial>& storeIn)
{
	storeIn = ReadObjMaterial(filename);
}


vector<ObjMaterial> ObjectFactory::ReadObjMaterial(string filename)
{
	vector<ObjMaterial> materialData;

	ifstream fileIn;
	string special = "", line = "";
	istringstream inputString;

	bool first = true;

	const unsigned int NAMELENGTH = 30;
	const unsigned int SPECIALCHARSIZE = 10;
	char specialChar[SPECIALCHARSIZE];

	char temp[512];

	ObjMaterial mat = DEFAULT_MATERIAL;
	//Set the default texture for materials without texturing
	//memcpy(&mat.texture, &DEFAULT_TEXTURE, sizeof(DEFAULT_TEXTURE));
	fileIn.open(filename, ios::in);
	if (fileIn.is_open())
	{
		while (std::getline(fileIn, line))
		{
			strncpy_s(temp, 512, line.c_str(), sizeof(temp));
			temp[sizeof(temp) - 1] = 0;

			inputString.str(line);
			if (line.substr(0, 6) == "newmtl")
			{
				if (!first)
				{
					ObjMaterial newMat = mat;
					materialData.push_back(newMat);
					mat = DEFAULT_MATERIAL;
					//Right now we push old settings to new materials, other alternative is setting new materials to default
					//memcpy(&mat.texture, &DEFAULT_TEXTURE, sizeof(DEFAULT_TEXTURE));
				}else
					first = false;
				// newmtl
				sscanf_s(temp, "%s %s\n", specialChar, SPECIALCHARSIZE, &mat.name, mat.MATERIAL_NAME_LENGTH);
			}
			else if (line.substr(0, 2) == "Kd")
			{
				// Kd
				sscanf_s(temp, "%s %f %f %f\n", specialChar, SPECIALCHARSIZE, &mat.Kd.x, &mat.Kd.y, &mat.Kd.z);
			}
			else if (line.substr(0, 2) == "Ka")
			{
				// Ka
				sscanf_s(temp, "%s %f %f %f\n", specialChar, SPECIALCHARSIZE, &mat.Ka.x, &mat.Ka.y, &mat.Ka.z);
			}
			else if (line.substr(0, 2) == "Ks")
			{
				// Ks
				sscanf_s(temp, "%s %f %f %f\n", specialChar, SPECIALCHARSIZE, &mat.Ks.x, &mat.Ks.y, &mat.Ks.z);
			}
			else if (line.substr(0, 2) == "Ns")
			{
				// Ns
				sscanf_s(temp, "%s %f", specialChar, SPECIALCHARSIZE, &mat.Ns);
			}
			else if (line.substr(0, 2) == "d ")
			{
				// d
				sscanf_s(temp, "%s %f", specialChar, SPECIALCHARSIZE, &mat.d);
			}
			else if (line.substr(0, 2) == "Tr")
			{
				// Tr which is the inverted version of 'd'
				sscanf_s(temp, "%s %f", specialChar, SPECIALCHARSIZE, &mat.d);
				// Invert the value for 'd'
				mat.d = 1 - mat.d;
			}
			else if (line.substr(0, 5) == "illum")
			{
				// illum
				sscanf_s(temp, "%s %i\n", specialChar, SPECIALCHARSIZE, &mat.illum);
			}
			else if (line.substr(0, 2) == "Tf")
			{
				// Tf
				//sscanf_s(temp, "%s %f %f %f\n", specialChar, SPECIALCHARSIZE, &mat.Tf.x, &mat.Tf.y, &mat.Tf.z);
			}
			else if (line.substr(0, 6) == "map_Kd")
			{
				// map_Kd
				string textureType = "";
				sscanf_s(temp, "%s %s\n", specialChar, SPECIALCHARSIZE, &mat.texture, mat.MATERIAL_NAME_LENGTH);
				textureType = mat.texture;
				textureType = textureType.substr(textureType.find('.', 0), 20);
				if (textureType.size() > 1)
					textureType = textureType.substr(1, 20);
				if (textureType == "jpg")
				{
					mat.textureFormat = TextureFormat::JPEG;
				}
				else if (textureType == "png")
				{
					mat.textureFormat = TextureFormat::PNG;
				}
				else if (textureType == "tga")
				{
					mat.textureFormat = TextureFormat::TARGA;
				}
			}
			//else if (line.substr(0, 2) == "Ni")
			//{
			//	// Ni
			//	sscanf_s(temp, "%s %i\n", specialChar, SPECIALCHARSIZE, &mat.Ni);
			//	materialData.push_back(mat);
			//}
		}
	}
	fileIn.close();
	ObjMaterial newMat = mat;
	materialData.push_back(newMat);

	return materialData;
}