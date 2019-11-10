#pragma once

#include "Transform.h"
#include "Mesh.h"
#include "Matrix4x4.h"


class GameObject
{
public:
	GameObject(){}

	Transform& GetTransform();
	Mesh& GetMesh();

private:
	Transform Transform;
	Mesh Mesh;
};