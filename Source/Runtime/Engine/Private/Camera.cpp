
#include "Precompiled.h"
#include "Camera.h"

void Camera::SetPosition(const Vector3 & InPosition)
{
	Position = InPosition;
}

void Camera::AddPosition(const Vector3 & InPosition)
{
	Position += InPosition;
}

void Camera::SetRotation(Vector3 InRotationDegree)
{
	Rotation = InRotationDegree;
}

void Camera::AddYawRotation(float InDegree)
{
	Rotation.Y += InDegree;
}

void Camera::AddRollRotation(float InDegree)
{
	Rotation.Z += InDegree;
}

void Camera::AddPitchRotation(float InDegree)
{
	Rotation.X += InDegree;
}


Matrix4x4 Camera::GetViewMat()
{
	Matrix4x4 tMat(Vector4::UnitX, Vector4::UnitY, Vector4::UnitZ, Vector4(-Position));
	float cy, sy, cp, sp, cr, sr;
	Math::GetSinCos(cy, sy, Rotation.Y);
	Math::GetSinCos(cp, sp, Rotation.X);
	Math::GetSinCos(cr, sr, Rotation.Z);

	Matrix4x4 rMat(
		Vector4(cr * cy + sr * sp * sy, cy * sr - cr * sp * sy, -cp * sy, false),
		Vector4(-sr * cp, cr * cp, -sp, false),
		Vector4(cr * sy - sr * sp * cy, sr * sy + cr * sp * cy, cp * cy, false),
		Vector4::UnitW);

	return rMat.Tranpose() * tMat;
}

Matrix4x4 Camera::GetPerspectiveMat()
{
	float d = 1.f / tanf(Math::Deg2Rad(FOV) * 0.5f);
	float repnf = 1.f / (n - f);
	float k = f * repnf;
	float l = f * n * repnf;

	Matrix4x4 pMat = Matrix4x4(
		Vector4::UnitX * repA * d,
		Vector4::UnitY * d,
		Vector4(0.f, 0.f, k, -1.f),
		Vector4(0.f, 0.f, l, 0.f));

	return pMat;
}

Matrix4x4 Camera::GetLookAtMat(Vector3 obj)
{
	Vector3 viewZ = (Position - obj).Normalize();
	Vector3 viewX = Vector3::UnitY.Cross(viewZ).Normalize();
	if (viewX.IsZero())
	{
		viewX = Vector3::UnitX;
	}
	Vector3 viewY = viewZ.Cross(viewX).Normalize();

	Matrix4x4 virMat = Matrix4x4(Vector4(viewX, false), Vector4(viewY, false), Vector4(viewZ, false), Vector4::UnitW).Tranpose();
	Matrix4x4 vitMat = Matrix4x4(Vector4::UnitX, Vector4::UnitY, Vector4::UnitZ, Vector4(-Position));

	return virMat * vitMat;
}
