
#include "Precompiled.h"
#include "SoftRendererImpl3D.h"
#include "SoftRenderer.h"

#include "GameObject.h"

SoftRendererImpl3D::SoftRendererImpl3D(SoftRenderer* InOwner)
{
	RSI = InOwner->RSI.get();
	ScreenSize = InOwner->CurrentScreenSize;
	InputManager = InOwner->GetInputManager();

	LoadResource();
	LoadScene();
}

SoftRendererImpl3D::~SoftRendererImpl3D()
{
}


void SoftRendererImpl3D::LoadResource()
{
	// Load Mesh Resource
	const int vertexCount = 24;
	Vector4* v = new Vector4[vertexCount]{
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

	const int triangleCount = 12;
	const int indexCount = triangleCount * 3;
	int* i = new int[indexCount]
	{
		0, 2, 1, 0, 3, 2,
			4, 6, 5, 4, 7, 6,
			8, 10, 9, 8, 11, 10,
			12, 14, 13, 12, 15, 14,
			16, 18, 17, 16, 19, 18,
			20, 22, 21, 20, 23, 22
	};

	cube.GetMesh().SetMesh(vertexCount, v,indexCount, i);
	box.GetMesh().SetMesh(vertexCount, v, indexCount, i);
	box2.GetMesh().SetMesh(vertexCount, v, indexCount, i);
}

void SoftRendererImpl3D::LoadScene()
{
	// Setup Game Scene 
	cube.GetTransform().SetPosition(Vector3::Zero);
	cube.GetTransform().SetScale(Vector3::One * 100.f);

	box.GetTransform().SetPosition(Vector3(200.f,0.f,0.f));
	box.GetTransform().SetScale(Vector3::One * 100.f);

	box2.GetTransform().SetPosition(Vector3(-100.f, 0.f, 0.f));
	box2.GetTransform().SetScale(Vector3::One * 50.f);

	camera.SetPosition(Vector3(0.f, 500.f, -500.f));
	camera.repA = (float)ScreenSize.Y / (float)ScreenSize.X;
}

void SoftRendererImpl3D::RenderFrameImpl()
{
	assert(RSI != nullptr && RSI->IsInitialized() && !ScreenSize.HasZero());

	Vector4* meshVertexBuffer = cube.GetMesh().GetVertexBuffer();
	int vc = cube.GetMesh().GetVertexCount();
	int ic = cube.GetMesh().GetIndexCount();
	VertexData* v = new VertexData[vc];
	for (int vi = 0; vi < vc; ++vi)
	{
		v[vi].Position = meshVertexBuffer[vi];
	}

	Matrix4x4 mMat = cube.GetTransform().GetTRS();
	Matrix4x4 vMat = camera.GetLookAtMat(cube.GetTransform().GetPosition());
	//Matrix4x4 vMat = camera.GetViewMat();
	Matrix4x4 pMat = camera.GetPerspectiveMat();

	Matrix4x4 m[3] = { pMat,vMat,mMat };
	RSI->SetUniformMatrix(m);
	RSI->SetVertexBuffer(v);
	RSI->SetIndexBuffer(cube.GetMesh().GetIndexBuffer());
	RSI->DrawPrimitive(vc, ic);

	delete[] v;

	//-------------
	Vector4* meshVertexBuffer2 = box.GetMesh().GetVertexBuffer();
	int vc2 = box.GetMesh().GetVertexCount();
	int ic2 = box.GetMesh().GetIndexCount();
	VertexData* v2 = new VertexData[vc2];
	for (int i = 0; i < vc2; ++i)
	{
		v2[i].Position = meshVertexBuffer2[i];
	}
	
//	vMat = camera.GetLookAtMat(box.GetTransform().GetPosition());

	Matrix4x4 m2[3] = { pMat,vMat,box.GetTransform().GetTRS() };
	RSI->SetUniformMatrix(m2);
	RSI->SetVertexBuffer(v2);
	RSI->SetIndexBuffer(box.GetMesh().GetIndexBuffer());
	RSI->DrawPrimitive(vc2, ic2);

	//FinalMatrix = camera.GetPerspectiveMat() * camera.GetLookAtMat(cube.GetTransform().GetPosition()) * cube.GetTransform().GetTRS();
	//Matrix4x4 mMat = cube.GetTransform().GetTRS();
	//Matrix4x4 vMat = camera.GetLookAtMat(cube.GetTransform().GetPosition());
	//Matrix4x4 pMat = camera.GetPerspectiveMat();
	//Matrix4x4 fMat = pMat * vMat * mMat;

	//Vector4* vertices = cube.GetMesh().GetVertexBuffer();
	//int* indexDatas = cube.GetMesh().GetIndexBuffer();

	//for (int t = 0; t < cube.GetMesh().GetTriangleSize(); t++)
	//{
	//	Vector4 tp[3];
	//	tp[0] = vertices[indexDatas[t * 3]];
	//	tp[1] = vertices[indexDatas[t * 3 + 1]];
	//	tp[2] = vertices[indexDatas[t * 3 + 2]];

	//	for (int ti = 0; ti < 3; ti++)
	//	{
	//		tp[ti] = fMat * tp[ti];
	//		float repW = 1.f / tp[ti].W;

	//		tp[ti].X *= repW;
	//		tp[ti].Y *= repW;
	//		tp[ti].Z *= repW;
	//	}

	//	//BackFace Culling
	//	Vector3 edge1 = (tp[1] - tp[0]).ToVector3();
	//	Vector3 edge2 = (tp[2] - tp[0]).ToVector3();

	//	Vector3 normal = -edge1.Cross(edge2).Normalize();
	//	
	//	Vector3 cameraDir = -Vector3::UnitZ;
	//	if (cameraDir.Dot(normal) <= 0.f)
	//	{
	//		// 종횡비가 다르기에 원래대로 맞춰주기 위해서 
	//		for (int ti = 0; ti < 3; ti++)
	//		{
	//			tp[ti].X *= (ScreenSize.X * 0.5f);
	//			tp[ti].Y *= (ScreenSize.Y * 0.5f);
	//		}

	//		RSI->DrawLine(tp[0].ToVector2(), tp[1].ToVector2(), LinearColor::Red);
	//		RSI->DrawLine(tp[0].ToVector2(), tp[2].ToVector2(), LinearColor::Red);
	//		RSI->DrawLine(tp[1].ToVector2(), tp[2].ToVector2(), LinearColor::Red);
	//	}
	//}

}

void SoftRendererImpl3D::UpdateImpl(float DeltaSeconds)
{
	// 모델링 변환 행렬.
	static float moveSpeed = 100.f;
	static float rotateSpeed = 180.f;

	Vector3 cubePos = Vector3::UnitZ * InputManager.GetYAxis() * moveSpeed * DeltaSeconds;
	float cubeRotationY = InputManager.GetXAxis() * rotateSpeed * DeltaSeconds;

	cube.GetTransform().AddPosition(cubePos);
	cube.GetTransform().AddYawRotation(cubeRotationY);

	box.GetTransform().AddPosition(cubePos);
//	box.GetTransform().AddYawRotation(cubeRotationY);

}
