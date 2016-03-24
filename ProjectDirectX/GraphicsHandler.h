#ifndef _GRAPHICSHANDLER_H_
#define _GRAPHICSHANDER_H_

//The class that handles direct x, I have forgotten why the 3 is there, possible refering to 3D
#include "d3dclass.h"
//The class that keeps track of the camera related data
#include "Camera.h"
//Old 2d model
#include "D2Object.h"
//New model for the ease of 2D rendering and visual effects in a 3d world
//Also usefull for post-processing
#include "QuadModel.h"
//New 3d model class and teh factory that creates it
#include "D3Object.h"
#include "ObjectFactory.h"
//For normal rendering
#include "TextureHandler.h"
//To be able to properly handle the inputs
#include "InputHandler.h"
//For deferred rendering
#include "DeferredBuffer.h"
#include "DeferredHandler.h"
#include "LightShader.h"


const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_FAR = 500.0f;
const float SCREEN_NEAR = 0.1f;

const float CAMERA_Y_UPPER_BOUND = 75;
const float CAMERA_Y_LOWER_BOUND = -75;

const Vector3 ORIG = { 0, 0, -10 };
const Vector3 D_FRONT = { 0, 0, 1 };
const Vector3 D_W = D_FRONT;
const Vector3 D_BACK = { 0, 0, -1 };
const Vector3 D_S = D_BACK;
const Vector3 D_LEFT = { -1, 0, 0 };
const Vector3 D_A = D_LEFT;
const Vector3 D_RIGHT = { 1, 0, 0 };
const Vector3 D_D = D_RIGHT;
const Vector3 D_UP = { 0, 2, 0 };

const int INPUT_CAP = 4;
const Vector3 DIRECTION_V[INPUT_CAP] = { D_FRONT, D_LEFT, D_BACK, D_RIGHT };

class GraphicsHandler
{
private:	//Variables
	//Ignore this object and just leave it to Kim Arvola Bjelkesten. It is just a handler for some directX related things
	D3DClass* m_Direct3D;
	//The camera
	Camera* m_Camera;
	//The objects in the scene
	std::vector<D3Object*> m_Models;
	//Our scene lights, all point lights and structs.
	//Extend to own class if other lights are needed.
	std::vector<LightStruct*> m_Lights;
	//Normal textured rendering
	TextureHandler* m_TextureShader;
	//Used for our deferred rendering and the post processing it is composed off
	QuadModel* m_FullScreenObject;
	//A object that will contain the data from the geometry call of deferred rendering
	DeferredBuffer* m_DeferredBuffers;
	//The deferred renderer
	DeferredHandler* m_DefferedShader;
	//The post processing done with the data from DefferedBuffers
	LightShader* m_LightShader;



	//A rotation value used for debugging
	float rotation;
public:
	GraphicsHandler();
	GraphicsHandler(const GraphicsHandler& other);
	~GraphicsHandler();

	bool Initialize(int screenWidth, int screenHeight, HWND hwnd);
	void ShutDown();
	bool Frame(int fps, float frameTime, InputHandler* inputObj);
	bool Render();
	bool RenderToDeferred();

private:	//Functions
	bool UpdateInput(InputHandler* inputObj, float dT);
	bool LoadScene(HWND hwnd);
};

#endif