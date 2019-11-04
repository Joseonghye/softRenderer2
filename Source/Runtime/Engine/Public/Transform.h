#pragma once

#include "Vector3.h"
#include "Matrix3x3.h"

class Transform
{
public:
	Matrix3x3 GetTRS() const;

	void SetPosition(const Vector3& InPosition);
	Vector3 GetPosition() { return Position; }

	void SetRotation(Vector3 InRotationDegree);
	void SetScale(const Vector3& InScale);

	void AddPosition(const Vector3& InPosition);
	void AddRotation(Vector3 InRotationDegree);
	void AddScale(const Vector3& InScale);

private:
	Vector3 Position = Vector3::Zero;
	Matrix4x4 Rotation;
	Vector3 Scale = Vector3::One;
};