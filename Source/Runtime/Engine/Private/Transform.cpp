
#include "Precompiled.h"
#include "Transform.h"


Matrix4x4 Transform::GetTRS() const
{
	Matrix4x4 tMat(Vector4::UnitX, Vector4::UnitY, Vector4::UnitZ, Vector4(Position));

	float cy, sy, cp, sp, cr, sr;
	Math::GetSinCos(cy, sy, Rotation.Y);
	Math::GetSinCos(cp, sp, Rotation.X);
	Math::GetSinCos(cr, sr, Rotation.Z);

	Matrix4x4 rMat = Matrix4x4(
		Vector4(cr * cy + sr * sp * sy, cy * sr - cr * sp * sy, -cp * sy, false),
		Vector4(-sr * cp, cr * cp, -sp, false),
		Vector4(cr * sy - sr * sp * cy, sr * sy + cr * sp * cy, cp * cy, false),
		Vector4::UnitW);

	Matrix4x4 sMat(Vector4::UnitX * Scale.X, Vector4::UnitY * Scale.Y, Vector4::UnitZ * Scale.Z, Vector4::UnitW);
	return tMat * rMat * sMat;
}

void Transform::SetPosition(const Vector3 & InPosition)
{
	Position = InPosition;
} 

void Transform::AddPosition(const Vector3 & InPosition)
{
	Position += InPosition;
}

void Transform::SetRotation(Vector3 InRotationDegree)
{
	Rotation = InRotationDegree;
}

void Transform::AddYawRotation(float InDegree)
{
	Rotation.Y += InDegree;
}

void Transform::AddRollRotation(float InDegree)
{
	Rotation.Z += InDegree;
}

void Transform::AddPitchRotation(float InDegree)
{
	Rotation.X += InDegree;
}

void Transform::SetScale(const Vector3 & InScale)
{
	Scale = InScale;
}


