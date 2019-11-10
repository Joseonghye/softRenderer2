
#include "Precompiled.h"
#include "Camera.h"

void Camera::SetPosition(const Vector3 & InPosition)
{
	Position = InPosition;
}

void Camera::SetRotation(Vector3 viewX, Vector3 viewY, Vector3 viewZ)
{
	Rotation = Matrix4x4(
					Vector4(viewX, false), Vector4(viewY, false), 
					Vector4(viewZ, false), Vector4::UnitW).Tranpose();
}

Matrix4x4 Camera::GetViewMat()
{
	Matrix4x4 tMat(Vector4::UnitX, Vector4::UnitY, Vector4::UnitZ, Vector4(-Position));
	
	return Rotation * tMat;
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
