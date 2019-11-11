#pragma once

#include "Vector3.h"
#include "Matrix4x4.h"

class Transform
{
public:
	Matrix4x4 GetTRS() const;

	void SetPosition(const Vector3& InPosition);
	Vector3 GetPosition() { return Position; }

	void SetRotation(Vector3 InRotationDegree);
	void SetScale(const Vector3& InScale);

	void AddPosition(const Vector3& InPosition);
	void AddScale(const Vector3& InScale);

private:
	Vector3 Position = Vector3::Zero;
	Matrix4x4 Rotation;
	Vector3 Scale = Vector3::One;
};