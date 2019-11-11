#pragma once

#include "Transform.h"
#include "Mesh.h"
#include "Matrix4x4.h"


class GameObject
{
public:
	GameObject(){}

	Transform& GetTransform() { return Transform; }
	//Mesh& GetMesh() { return Mesh; }
	void SetMesh(Mesh* InMeshPtr) { ShareMeshPtr = InMeshPtr; }

private:
	Transform Transform;
	//Mesh Mesh;
	Mesh* ShareMeshPtr = nullptr;
};