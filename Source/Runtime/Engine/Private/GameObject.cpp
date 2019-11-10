
#include "Precompiled.h"
#include "GameObject.h"

Transform & GameObject::GetTransform()
{
	return Transform;
}

Mesh & GameObject::GetMesh()
{
	return Mesh;
}
