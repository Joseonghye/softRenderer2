
#include "Precompiled.h"
#include "Transform.h"


Matrix4x4 Transform::GetTRS() const
{
	Matrix4x4 tMat(Vector4::UnitX, Vector4::UnitY, Vector4::UnitZ, Vector4(Position));
//	Matrix3x3 rMat = Matrix3x3(Vector3(cosf(rad), sinf(rad), 0.f), Vector3(-sinf(rad), cosf(rad), 0.f), Vector3::UnitZ);
	Matrix4x4 sMat = Matrix4x4(Vector3::UnitX * Scale.X, Vector3::UnitY * Scale.Y, Vector3::UnitZ * Scale.Z, Vector4::UnitW);
	return tMat *Rotation * sMat;
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

void Transform::AddScale(const Vector3 & InScale)
{
	Scale += InScale;
}
