#pragma once

#include "Transform.h"

class Camera
{
public:
	Camera() = default;

public:
	Transform& GetTransform() { return Transform; }
	Matrix4x4 GetLookAtMatrix(GameObject& InTargetObject);
	Matrix4x4 GetPerspectiveMatrix(int InScreenSizeX, int InScreenSizeY);
	float GetFOV() const { return FOV; }
	float GetNearZ() const { return NearZ; }
	float GetFarZ() const { return FarZ; }

private:
	Transform Transform;
	float FOV = 60.f;
	float NearZ = 5.5f;
	float FarZ = 1000.f;
};

