
#include "Precompiled.h"
#include "Transform.h"


Matrix3x3 Transform::GetTRS() const
{
	Matrix3x3 tMat = Matrix3x3(Vector3::UnitX, Vector3::UnitY, Vector3(Position.X, Position.Y, Position.Z));
//	Matrix3x3 rMat = Matrix3x3(Vector3(cosf(rad), sinf(rad), 0.f), Vector3(-sinf(rad), cosf(rad), 0.f), Vector3::UnitZ);
	Matrix3x3 sMat = Matrix3x3(Vector3::UnitX * Scale.X, Vector3::UnitY * Scale.Y, Vector3::UnitZ * Scale.Z);
	return tMat * sMat;
}

void Transform::SetPosition(const Vector3 & InPosition)
{
	Position = InPosition;
}

void Transform::SetRotation(Vector3 InRotationDegree)
{
	float cy, sy, cp, sp, cr, sr;
	Math::GetSinCos(cy, sy, InRotationDegree.Y);
	Math::GetSinCos(cp, sp, InRotationDegree.X);
	Math::GetSinCos(cr, sr, InRotationDegree.Z);

	Rotation = Matrix4x4 (
		Vector4(cr * cy + sr * sp * sy, cy * sr - cr * sp * sy, -cp * sy, false),
		Vector4(-sr * cp, cr * cp, -sp, false),
		Vector4(cr * sy - sr * sp * cy, sr * sy + cr * sp * cy, cp * cy, false),
		Vector4::UnitW);
}

void Transform::SetScale(const Vector3 & InScale)
{
	Scale = InScale;
}

void Transform::AddPosition(const Vector3 & InPosition)
{
	Position += InPosition;
}

void Transform::AddRotation(Vector3 InRotationDegree)
{
}

void Transform::AddScale(const Vector3 & InScale)
{
	Scale += InScale;
}
