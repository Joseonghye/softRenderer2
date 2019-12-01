#pragma once

#include "Transform.h"
#include "Mesh.h"

class GameObject
{
public:
	GameObject() = default;
	~GameObject() {};

	Transform& GetTransform() { return Transform; }
	void SetMesh(Mesh* InMeshPtr);
	Mesh* GetMesh() { return SharedMeshPtr; }

private:
	Transform Transform;
	Mesh* SharedMeshPtr = nullptr;
};

