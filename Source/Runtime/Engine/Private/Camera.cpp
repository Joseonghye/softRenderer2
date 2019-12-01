
#include "Precompiled.h"
#include "Camera.h"
#include "GameObject.h"

Matrix4x4 Camera::GetLookAtMatrix(GameObject & InTargetObject)
{
	Vector3 viewZ = (Transform.Position - InTargetObject.GetTransform().Position).Normalize();
	Vector3 viewX = Vector3::UnitY.Cross(viewZ).Normalize();
	if (viewX.IsZero())
	{
		viewX = Vector3::UnitX;
	}
	Vector3 viewY = viewZ.Cross(viewX).Normalize();

	Matrix4x4 virMat = Matrix4x4(Vector4(viewX, false), Vector4(viewY, false), Vector4(viewZ, false), Vector4::UnitW).Tranpose();
	Matrix4x4 vitMat = Matrix4x4(Vector4::UnitX, Vector4::UnitY, Vector4::UnitZ, Vector4(-Transform.Position));
	return virMat * vitMat;
}

Matrix4x4 Camera::GetPerspectiveMatrix(int InScreenSizeX, int InScreenSizeY)
{
	// 투영 행렬. 깊이 값의 범위는 0~1
	static float invA = (float)InScreenSizeY / (float)InScreenSizeX;
	static float d = 1.f / tanf(Math::Deg2Rad(FOV) * 0.5f);
	static float n = 5.5f;
	static float f = 100.f;
	float invNF = 1.f / (n - f);
	float k = f * invNF;
	float l = f * n * invNF;
	return Matrix4x4(
		Vector4::UnitX * invA * d, 
		Vector4::UnitY * d, 
		Vector4(0.f, 0.f, k, -1.f), 
		Vector4(0.f, 0.f, l, 0.f));
}
