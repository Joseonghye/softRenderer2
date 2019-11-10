
#include "Precompiled.h"
#include "SoftRendererImpl3D.h"
#include "SoftRenderer.h"

#include "GameObject.h"

SoftRendererImpl3D::SoftRendererImpl3D(SoftRenderer* InOwner)
{
	RSI = InOwner->RSI.get();
	ScreenSize = InOwner->CurrentScreenSize;
	InputManager = InOwner->GetInputManager();
}

SoftRendererImpl3D::~SoftRendererImpl3D()
{
}


void SoftRendererImpl3D::RenderFrameImpl()
{
	assert(RSI != nullptr && RSI->IsInitialized() && !ScreenSize.HasZero());

	const int vertexCount = 24;
	 Vector4 v[vertexCount] =
	 {
		// Front 
		Vector4(0.5f, -0.5f, 0.5f),
		Vector4(0.5f, 0.5f, 0.5f),
		Vector4(0.5f, 0.5f, -0.5f),
		Vector4(0.5f, -0.5f, -0.5f),
		// Left
		Vector4(-0.5f, -0.5f, 0.5f),
		Vector4(-0.5f, 0.5f, 0.5f),
		Vector4(0.5f, 0.5f, 0.5f),
		Vector4(0.5f, -0.5f, 0.5f),
		// Right
		Vector4(0.5f, -0.5f, -0.5f),
		Vector4(0.5f, 0.5f, -0.5f),
		Vector4(-0.5f, 0.5f, -0.5f),
		Vector4(-0.5f, -0.5f, -0.5f),
		// Back
		Vector4(-0.5f, -0.5f, -0.5f),
		Vector4(-0.5f, 0.5f, -0.5f),
		Vector4(-0.5f, 0.5f, 0.5f),
		Vector4(-0.5f, -0.5f, 0.5f),
		// Top
		Vector4(0.5f, 0.5f, 0.5f),
		Vector4(-0.5f, 0.5f, 0.5f),
		Vector4(-0.5f, 0.5f, -0.5f),
		Vector4(0.5f, 0.5f, -0.5f),
		// Bottom
		Vector4(-0.5f, -0.5f, 0.5f),
		Vector4(0.5f, -0.5f, 0.5f),
		Vector4(0.5f, -0.5f, -0.5f),
		Vector4(-0.5f, -0.5f, -0.5f)
	};

	cube.GetMesh().SetVertex(vertexCount,v);

	const int triangleCount = 12;
	const int indexCount = triangleCount * 3;
	static int i[indexCount] =
	{
	 0, 2, 1, 0, 3, 2,
	 4, 6, 5, 4, 7, 6,
	 8, 10, 9, 8, 11, 10,
	 12, 14, 13, 12, 15, 14,
	 16, 18, 17, 16, 19, 18,
	 20, 22, 21, 20, 23, 22
	};

	cube.GetMesh().SetIndex(triangleCount, i);

	
	Vector3 camera = -Vector3::UnitZ;

	for (int t = 0; t < cube.GetMesh().GetTriangleSize(); t++)
	{
		Vector4 tp[3];
		int num[3] = { cube.GetMesh().Indices[t * 3] ,cube.GetMesh().Indices[t * 3 + 1],cube.GetMesh().Indices[t * 3 + 2] };
		tp[0] = cube.GetMesh().Verties[num[0]];
		tp[1] = cube.GetMesh().Verties[num[1]];
		tp[2] = cube.GetMesh().Verties[num[2]];

		for (int ti = 0; ti < 3; ti++)
		{
			tp[ti] = FinalMatrix * tp[ti];
		//	tp[ti] = tp[ti] * (1/tp[ti].W);
		
			tp[ti].X = tp[ti].X / tp[ti].W;
			tp[ti].Y = tp[ti].Y / tp[ti].W;
			tp[ti].Z = tp[ti].Z / tp[ti].W;
		}

		//BackFace Culling
		Vector3 edge1 = (tp[1] - tp[0]).ToVector3();
		Vector3 edge2 = (tp[2] - tp[0]).ToVector3();

		Vector3 normal = -edge1.Cross(edge2).Normalize();
		
		if (normal.Dot(camera) <= 0.f)
		{
			// 종횡비가 다르기에 원래대로 맞춰주기 위해서 
			for (int ti = 0; ti < 3; ti++)
			{
				tp[ti].X *= (ScreenSize.X * 0.5f);
				tp[ti].Y *= (ScreenSize.Y * 0.5f);
			}

			RSI->DrawLine(tp[0].ToVector2(), tp[1].ToVector2(), LinearColor::Red);
			RSI->DrawLine(tp[0].ToVector2(), tp[2].ToVector2(), LinearColor::Red);
			RSI->DrawLine(tp[1].ToVector2(), tp[2].ToVector2(), LinearColor::Red);
		}
	}

}

void SoftRendererImpl3D::UpdateImpl(float DeltaSeconds)
{
	// 모델링 변환 행렬.
	static Vector3 cubePos = Vector3::Zero;
	static float cubeRotationY = 0.f;
	static float cubeRotationX = 0.f;
	static float cubeRotationZ = 0.f;
	static float cubeScaleXYZ = 100.f;

	static float moveSpeed = 100.f;
	static float rotateSpeed = 180.f;
	cubePos += Vector3::UnitZ * InputManager.GetYAxis() * moveSpeed * DeltaSeconds;
	cubeRotationY += InputManager.GetXAxis() * rotateSpeed * DeltaSeconds;

	
	cube.GetTransform().SetPosition(cubePos);
	cube.GetTransform().SetRotation(Vector3(cubeRotationX, cubeRotationY, cubeRotationZ));
	cube.GetTransform().SetScale(Vector3(cubeScaleXYZ, cubeScaleXYZ, cubeScaleXYZ));

	/*float cy, sy, cp, sp, cr, sr;
	Math::GetSinCos(cy, sy, cubeRotationY);
	Math::GetSinCos(cp, sp, cubeRotationX);
	Math::GetSinCos(cr, sr, cubeRotationZ);

	Matrix4x4 tMat(Vector4::UnitX, Vector4::UnitY, Vector4::UnitZ, Vector4(cubePos));
	Matrix4x4 rMat(
		Vector4(cr * cy + sr * sp * sy, cy * sr - cr * sp * sy, -cp * sy, false),
		Vector4(-sr * cp, cr * cp, -sp, false),
		Vector4(cr * sy - sr * sp * cy, sr * sy + cr * sp * cy, cp * cy, false),
		Vector4::UnitW);

	Matrix4x4 sMat(Vector4::UnitX * cubeScaleXYZ, Vector4::UnitY * cubeScaleXYZ, Vector4::UnitZ * cubeScaleXYZ, Vector4::UnitW);
	Matrix4x4 mMat = tMat * rMat * sMat;*/

	// 카메라 변환 행렬.
	camera.SetPosition(Vector3(0.f, 500.f, -500.f));
	static Vector3 cameraPos(0.f, 500.f, -500.f);

	Vector3 viewZ = (camera.GetPosition() - cube.GetTransform().GetPosition()).Normalize();
	Vector3 viewX = Vector3::UnitY.Cross(viewZ).Normalize();
	if (viewX.IsZero())
	{
		viewX = Vector3::UnitX;
	}
	Vector3 viewY = viewX.Cross(viewZ).Normalize();

	camera.SetRotation(viewX, viewY, viewZ);
	Matrix4x4 virMat = Matrix4x4(Vector4(viewX, false), Vector4(viewY, false), Vector4(viewZ, false), Vector4::UnitW).Tranpose();
	Matrix4x4 vitMat = Matrix4x4(Vector4::UnitX, Vector4::UnitY, Vector4::UnitZ, Vector4(-cameraPos));
	Matrix4x4 vMat = virMat * vitMat;

	// 투영 행렬
	static float repA = (float)ScreenSize.Y / (float)ScreenSize.X;
	static float d = 1.f / tanf(Math::Deg2Rad(FOV) * 0.5f);
	static float n = 5.5f;
	static float f = 100.f;
	static float repnf = 1 / (n - f);

	Matrix4x4 pMat = Matrix4x4(
		Vector4(d*repA, 0.f, 0.f, false),
		Vector4(0.f, d, 0.f, false), 
		Vector4(0.f, 0.f, (n + f)* repnf,-1.f ), 
		Vector4(0.f, 0.f, (2 * n*f)*repnf, 0.f));

	camera.repA = (float)ScreenSize.Y / (float)ScreenSize.X;
	// 최종 행렬
	//FinalMatrix = pMat * vMat * cube.GetTransform().GetTRS(); //* mMat;
	FinalMatrix = camera.GetPerspectiveMat() * camera.GetViewMat() * cube.GetTransform().GetTRS();
}
