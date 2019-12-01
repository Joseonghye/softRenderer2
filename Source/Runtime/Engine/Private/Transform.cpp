
#include "Precompiled.h"
#include "Transform.h"

void Transform::SetPosition(const Vector3 & InPosition)
{
	Position = InPosition;
}

void Transform::AddPosition(const Vector3 & InPosition)
{
	Position += InPosition;
}

void Transform::SetRotation(const Rotator & InRotation)
{
	Rotation = InRotation;
	CalcLocalAxis();
}

void Transform::AddYawRotation(float InDegree)
{
	if (InDegree == 0.f)
	{
		return;
	}

	Rotation.Yaw += InDegree;
	CalcLocalAxis();
}

void Transform::AddRollRotation(float InDegree)
{
	if (InDegree == 0.f)
	{
		return;
	}

	Rotation.Roll += InDegree;
	CalcLocalAxis();
}


void Transform::AddPitchRotation(float InDegree)
{
	if (InDegree == 0.f)
	{
		return;
	}

	Rotation.Pitch += InDegree;
	CalcLocalAxis();
}

void Transform::SetScale(const Vector3 & InScale)
{
	Scale = InScale;
}

Matrix4x4 Transform::GetModelingMatrix() const
{
	Matrix4x4 tMat(Vector4::UnitX, Vector4::UnitY, Vector4::UnitZ, Vector4(Position));
	Matrix4x4 rMat(
		Vector4(Right, false),
		Vector4(Up, false),
		Vector4(Forward, false),
		Vector4::UnitW);

	Matrix4x4 sMat(Vector4::UnitX * Scale.X, Vector4::UnitY * Scale.Y, Vector4::UnitZ * Scale.Z, Vector4::UnitW);
	return tMat * rMat * sMat;
}

void Transform::CalcLocalAxis()
{
	float cy, sy, cp, sp, cr, sr;
	Math::GetSinCos(sy, cy, Rotation.Yaw);
	Math::GetSinCos(sp, cp, Rotation.Pitch);
	Math::GetSinCos(sr, cr, Rotation.Roll);

	Right = Vector3(cy * cr + sy * sp * sr, cp * sr, -sy * cr + cy * sp * sr);
	Up = Vector3(-cy * sr + sy * sp * cr, cp * cr, sy * sr + cy * sp * cr);
	Forward = Vector3(sy * cp, -sp, cy * cp);
}
