
#include "Precompiled.h"
#include "Camera.h"

void Camera::SetPosition(const Vector3 & InPosition)
{
	Position = InPosition;
}

void Camera::SetRotation(Vector3 InRotationDegree)
{
	float cy, sy, cp, sp, cr, sr;
	Math::GetSinCos(cy, sy, InRotationDegree.Y);
	Math::GetSinCos(cp, sp, InRotationDegree.X);
	Math::GetSinCos(cr, sr, InRotationDegree.Z);

	Rotation = Matrix4x4(
		Vector4(cr * cy + sr * sp * sy, cy * sr - cr * sp * sy, -cp * sy, false),
		Vector4(-sr * cp, cr * cp, -sp, false),
		Vector4(cr * sy - sr * sp * cy, sr * sy + cr * sp * cy, cp * cy, false),
		Vector4::UnitW);
}

Matrix4x4 Camera::GetViewMat()
{
	Matrix4x4 tMat(Vector4::UnitX, Vector4::UnitY, Vector4::UnitZ, Vector4(-Position));
	
	return Rotation.Tranpose() * tMat;
}

Matrix4x4 Camera::GetPerspectiveMat()
{
	float d = 1.f / tanf(Math::Deg2Rad(FOV) * 0.5f);
	float repnf = 1 / (n - f);

	Matrix4x4 pMat = Matrix4x4(
		Vector4(d*repA, 0.f, 0.f, false),
		Vector4(0.f, d, 0.f, false),
		Vector4(0.f, 0.f, (n + f)* repnf, -1.f),
		Vector4(0.f, 0.f, (2 * n*f)*repnf, 0.f));

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
	Vector3 viewY = viewX.Cross(viewZ).Normalize();

	Matrix4x4 virMat = Matrix4x4(Vector4(viewX, false), Vector4(viewY, false), Vector4(viewZ, false), Vector4::UnitW).Tranpose();
	Matrix4x4 vitMat = Matrix4x4(Vector4::UnitX, Vector4::UnitY, Vector4::UnitZ, Vector4(-Position));

	return virMat * vitMat;
}
