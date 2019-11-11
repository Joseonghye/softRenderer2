#pragma once
#include "Transform.h"
#include "Matrix4x4.h"

class Camera
{
public:
	void SetPosition(const Vector3& InPosition);
	void AddPosition(const Vector3 & InPosition);
	Vector3 GetPosition() { return Position; }

	void SetRotation(Vector3 InRotationDegree);
	void AddYawRotation(float InDegree);
	void AddRollRotation(float InDegree);
	void AddPitchRotation(float InDegree);

	Matrix4x4 GetViewMat();
	Matrix4x4 GetPerspectiveMat();
	Matrix4x4 GetLookAtMat(Vector3 obj);

	//시야각
	float FOV = 60.f;
	//근평면 원평면
	float n = 5.5f;
	float f = 100.f;

	float repA;

private:
	Vector3 Position = Vector3::Zero;
	Vector3 Rotation = Vector3::Zero;
};