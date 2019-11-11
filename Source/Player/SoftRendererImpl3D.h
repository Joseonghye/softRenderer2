#pragma once

#include "RenderingSoftwareInterface.h"
#include "InputManager.h"
#include "GameObject.h"
#include "Camera.h"

class SoftRendererImpl3D
{
public:
	SoftRendererImpl3D(class SoftRenderer* InOwner);
	~SoftRendererImpl3D();

public:
	void LoadResource();
	void LoadScene();
	void RenderFrameImpl();
	void UpdateImpl(float DeltaSeconds);

private:
	RenderingSoftwareInterface* RSI = nullptr;

	GameObject cube;
	GameObject box;
	GameObject box2;

	Camera camera;

	// 2D Grid Options
	int Grid2DUnit = 10;
	ScreenPoint ScreenSize;

	// Input Manager
	InputManager InputManager;

};
