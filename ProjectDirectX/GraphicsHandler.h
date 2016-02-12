#ifndef _GRAPHICSHANDLER_H_
#define _GRAPHICSHANDER_H_

#include "d3dclass.h"
#include "Camera.h"
#include "D2Object.h"
#include "D3Object.h"
#include "ObjectFactory.h"
#include "TextureHandler.h"
#include "InputHandler.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

const Vector3 ORIG = { 0, 0, -10 };
const Vector3 D_UP = { 0, 0, 1 };
const Vector3 D_W = D_UP;
const Vector3 D_DOWN = { 0, 0, -1 };
const Vector3 D_S = D_DOWN;
const Vector3 D_LEFT = { -1, 0, 0 };
const Vector3 D_A = D_LEFT;
const Vector3 D_RIGHT = { 1, 0, 0 };
const Vector3 D_D = D_RIGHT;

const int INPUT_CAP = 4;
const Vector3 DIRECTION_V[INPUT_CAP] = { D_UP, D_LEFT, D_DOWN, D_RIGHT };

class GraphicsHandler
{
private:
	D3DClass* m_Direct3D;
	Camera* m_Camera;
	D3Object* m_Model;
	TextureHandler* m_TextureShader;
	float rotation;
	//ShaderHandler* m_shaderHandler;
public:
	GraphicsHandler();
	GraphicsHandler(const GraphicsHandler& other);
	~GraphicsHandler();

	bool Initialize(int screenWidth, int screenHeight, HWND hwnd);
	void ShutDown();
	bool Frame(int fps, float frameTime, InputHandler* inputObj);
	bool Render();

private:
	bool UpdateInput(InputHandler* inputObj, float dT);
};

#endif