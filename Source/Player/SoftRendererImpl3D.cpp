
#include "Precompiled.h"
#include "SoftRendererImpl3D.h"
#include "SoftRenderer.h"

SoftRendererImpl3D::SoftRendererImpl3D(SoftRenderer* InOwner)
{
	RSI = InOwner->RSI.get();
	ScreenSize = InOwner->CurrentScreenSize;
	InputManager = InOwner->GetInputManager();

	LoadResource();
	LoadScene();

	Owner = InOwner;
}

SoftRendererImpl3D::~SoftRendererImpl3D()
{
	//for (auto& g : Scene)
	//{
	//	g.GetMesh().ReleaseAllBuffers();
	//}
}

void SoftRendererImpl3D::LoadResource()
{
	// Load Mesh Resource
	const int vertexCount = 24;
	const int triangleCount = 12;
	const int indexCount = triangleCount * 3;

	Vector4* v = new Vector4[vertexCount] {
		// Right 
		Vector4(0.5f, -0.5f, 0.5f),
		Vector4(0.5f, 0.5f, 0.5f),
		Vector4(0.5f, 0.5f, -0.5f),
		Vector4(0.5f, -0.5f, -0.5f),
		// Front
		Vector4(-0.5f, -0.5f, 0.5f),
		Vector4(-0.5f, 0.5f, 0.5f),
		Vector4(0.5f, 0.5f, 0.5f),
		Vector4(0.5f, -0.5f, 0.5f),
		// Back
		Vector4(0.5f, -0.5f, -0.5f),
		Vector4(0.5f, 0.5f, -0.5f),
		Vector4(-0.5f, 0.5f, -0.5f),
		Vector4(-0.5f, -0.5f, -0.5f),
		// Left
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

	Color32* c = new Color32[vertexCount] {
		// Right
		Color32(0, 0, 0),
		Color32(0, 0, 0),
		Color32(0, 0, 0),
		Color32(0, 0, 0),
		// Front
		Color32(0, 0, 0),
		Color32(0, 0, 0),
		Color32(0, 0, 0),
		Color32(0, 0, 0),
		// Back
		Color32(0, 0, 0),
		Color32(0, 0, 0),
		Color32(0, 0, 0),
		Color32(0, 0, 0),
		// Left
		Color32(0, 0, 0),
		Color32(0, 0, 0),
		Color32(0, 0, 0),
		Color32(0, 0, 0),
		// Top
		Color32(0, 0, 0),
		Color32(0, 0, 0),
		Color32(0, 0, 0),
		Color32(0, 0, 0),
		// Bottom
		Color32(0, 0, 0),
		Color32(0, 0, 0),
		Color32(0, 0, 0),
		Color32(0, 0, 0)
	};

	int* i = new int[indexCount] {
	 0, 2, 1, 0, 3, 2,
	 4, 6, 5, 4, 7, 6,
	 8, 10, 9, 8, 11, 10,
	 12, 14, 13, 12, 15, 14,
	 16, 18, 17, 16, 19, 18,
	 20, 22, 21, 20, 23, 22
	};

	SharedCubeMesh = std::make_unique<Mesh>();
	SharedCubeMesh.get()->SetMesh(vertexCount, indexCount, v, i);
	SharedCubeMesh.get()->SetColor(c);

	delete[] v;
	delete[] i;
}

void SoftRendererImpl3D::LoadScene()
{
	// Setup Game Scene 
	GameObject* player = new GameObject();
	player->SetMesh(SharedCubeMesh.get());
	player->GetTransform().SetPosition(Vector3::Zero);
	player->GetTransform().SetRotation(Rotator(0.f, 0.f, 0.f));
	player->GetTransform().SetScale(Vector3::One * 100.f);
	Scene.emplace_back(player);

	GameObject* frustumTestObject1 = new GameObject();
	frustumTestObject1->SetMesh(SharedCubeMesh.get());
	frustumTestObject1->GetTransform().SetPosition(Vector3(0.f, 0.f, -600.f));
	frustumTestObject1->GetTransform().SetScale(Vector3::One * 100.f);

	GameObject* frustumTestObject2 = new GameObject();
	frustumTestObject2->SetMesh(SharedCubeMesh.get());
	frustumTestObject2->GetTransform().SetPosition(Vector3(-400.f, 0.f, 0.f));
	frustumTestObject2->GetTransform().SetScale(Vector3::One * 100.f);

	GameObject* frustumTestObject3 = new GameObject();
	frustumTestObject3->SetMesh(SharedCubeMesh.get());
	frustumTestObject3->GetTransform().SetPosition(Vector3(400.f, 0.f, 0.f));
	frustumTestObject3->GetTransform().SetScale(Vector3::One * 100.f);

	GameObject* frustumTestObject4 = new GameObject();
	frustumTestObject4->SetMesh(SharedCubeMesh.get());
	frustumTestObject4->GetTransform().SetPosition(Vector3(0.f, 0.f, 600.f));
	frustumTestObject4->GetTransform().SetScale(Vector3::One * 100.f);

	GameObject* frustumTestObject5 = new GameObject();
	frustumTestObject5->SetMesh(SharedCubeMesh.get());
	frustumTestObject5->GetTransform().SetPosition(Vector3(0.f, 400.f, 0.f));
	frustumTestObject5->GetTransform().SetScale(Vector3::One * 100.f);

	GameObject* frustumTestObject6 = new GameObject();
	frustumTestObject6->SetMesh(SharedCubeMesh.get());
	frustumTestObject6->GetTransform().SetPosition(Vector3(0.f, -400.f, 0.f));
	frustumTestObject6->GetTransform().SetScale(Vector3::One * 100.f);

	Scene.emplace_back(frustumTestObject1);
	Scene.emplace_back(frustumTestObject2);
	Scene.emplace_back(frustumTestObject3);
	Scene.emplace_back(frustumTestObject4);
	Scene.emplace_back(frustumTestObject5);
	Scene.emplace_back(frustumTestObject6);

	Camera.GetTransform().SetPosition(Vector3(0.f, 0.f, -500.f));
}

void SoftRendererImpl3D::RenderFrameImpl()
{
	assert(RSI != nullptr && RSI->IsInitialized() && !ScreenSize.HasZero());

	auto player = Scene[0].get();
	Matrix4x4 vMat = Camera.GetLookAtMatrix(*player);
	Matrix4x4 pMat = Camera.GetPerspectiveMatrix(ScreenSize.X, ScreenSize.Y);

	float halfFOV = Camera.GetFOV() * 0.5f;
	float tanHalfFOV = tanf(Math::Deg2Rad(halfFOV));
	int cullingCount = 0;

	for (auto& g : Scene)
	{
		// 현재 게임 오브젝트의 뷰 좌표계 값을 구하기.
		Vector3 viewPos = vMat * g->GetTransform().GetPosition();

		// 게임 오브젝트의 메시를 둘러싼 구체의 반지름을 구함.
		float r = g->GetMesh()->GetSphereArea().Radius * g->GetTransform().GetScale().GetMax();

		float d = viewPos.X + viewPos.Y + viewPos.Z;
		/*
		Vector3 n = Vector3(-1.0f, 0.0f, tanHalfFOV).Normalize();
		float _D = n.Dot(viewPos);
		
		Vector3 n2 = Vector3(1.0f, 0.0f, -tanHalfFOV).Normalize();
		float _D2 = n2.Dot(viewPos);*/

		Vector3 n[6] = {
			Vector3::UnitZ,
			-Vector3::UnitZ,
			Vector3(-1.0f , 0.0f, tanHalfFOV).Normalize(),
			Vector3(1.0f, 0.0f, -tanHalfFOV).Normalize(),
			Vector3(0.0f, 1.0f, -tanHalfFOV).Normalize(),
			Vector3(0.0f, -1.0f, tanHalfFOV).Normalize()
		};

		Vector3 camPos = Camera.GetTransform().GetPosition();

		// 1. Frustum 평면 구하기
		Plane frustumPlanes[6];
		for (int i = 0; i < 6; i++)
		{
			float D = n[i].Dot(camPos);
			frustumPlanes[i] = Plane(n[i], -D).Normalize();
		}

		bool isInside = true;
		for (int p = 0; p < 6; ++p)
		{
			float e = frustumPlanes[p].Normal.Dot(viewPos); //2. 평면의 노멀에 투영된 벡터를 사용해 원점부터의 거리 구하기.
			if (e - r + frustumPlanes[p].D > 0.0f)  // 3. 세 가지 정보를 활용해 평면 판정을 수행하기
			{
				isInside = false;
				cullingCount++;
				break;
			}
		}

		if (!isInside)
		{
			// 컬링 바깥쪽에 있으면 그리지 않고 통과하기.
			continue;
		}


		Matrix4x4 mMat = g->GetTransform().GetModelingMatrix();

		Matrix4x4 m[3] = { mMat, vMat, pMat };

		Mesh* meshToRender = player->GetMesh();
		int vertexCount = meshToRender->GetVertexCount();
		int indexCount = meshToRender->GetIndexCount();
		Vector4* meshVertexBuffer = meshToRender->GetVertexBuffer();
		Color32* meshColorBuffer = meshToRender->GetColorBuffer();
		int* i = meshToRender->GetIndexBuffer();
		VertexData* v = new VertexData[vertexCount];
		for (int vi = 0; vi < vertexCount; ++vi)
		{
			v[vi].Position = meshVertexBuffer[vi];
			v[vi].Color = LinearColor(meshColorBuffer[vi]);
		}

		RSI->SetUniformMatrix(m);
		RSI->SetVertexBuffer(v);
		RSI->SetIndexBuffer(i);
		RSI->DrawTrianglePrimitive(vertexCount, indexCount);

		delete[] v;
	}

	Owner->SetCullingCount(cullingCount);
	DrawGizmo3D(vMat, pMat);
}

void SoftRendererImpl3D::UpdateImpl(float DeltaSeconds)
{
	// 게임 로직.
	static float moveSpeed = 1000.f;
	static float rotateSpeed = 180.f;

	auto player = Scene[0].get();
	Vector3 deltaPos = Vector3::UnitZ * InputManager.GetYAxis() * moveSpeed * DeltaSeconds;
	float deltaDegree = InputManager.GetXAxis() * rotateSpeed * DeltaSeconds;
	float deltaDegree2 = InputManager.GetYAxis() * rotateSpeed * DeltaSeconds;
	//player->GetTransform().AddPosition(deltaPos);
	player->GetTransform().AddYawRotation(deltaDegree);
}

void SoftRendererImpl3D::DrawGizmo3D(Matrix4x4 InVMatrix, Matrix4x4 InPMatrix)
{
	const int gizmosVertexCount = 6;
	VertexData v[gizmosVertexCount] = {
		VertexData(Vector3::Zero, LinearColor::Red),
		VertexData(Vector3::UnitX * 500.f, LinearColor::Red),
		VertexData(Vector3::Zero, LinearColor::Green),
		VertexData(Vector3::UnitY * 500.f, LinearColor::Green),
		VertexData(Vector3::Zero, LinearColor::Blue),
		VertexData(Vector3::UnitZ * 500.f, LinearColor::Blue),
	};

	const int gizmosLineCount = 3;
	const int gizmosIndexCount = gizmosLineCount * 2;
	int i[gizmosIndexCount] = {
		0, 1,
		2, 3,
		4, 5
	};

	Matrix4x4 m[3] = { Matrix4x4(), InVMatrix, InPMatrix };

	RSI->SetUniformMatrix(m);
	RSI->SetVertexBuffer(v);
	RSI->SetIndexBuffer(i);
	RSI->DrawLinePrimitive(gizmosVertexCount, gizmosIndexCount);
}
