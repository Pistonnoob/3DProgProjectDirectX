#ifndef _GRAPHICSHANDLER_H_
#define _GRAPHICSHANDER_H_

#include "d3dclass.h"
#include "Camera.h"
#include "D2Object.h"
#include "D3Object.h"
#include "TextureHandler.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


class GraphicsHandler
{
private:
	D3DClass* m_Direct3D;
	bool Render();
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
	bool Frame();
};

#endif