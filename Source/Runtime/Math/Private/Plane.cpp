#include "Precompiled.h"
#include "Plane.h"

Plane::Plane(const Vector3 & InP1, const Vector3 & InP2, const Vector3 & InP3)
{
	Vector3 v1 = InP2 - InP1;
	Vector3 v2 = InP3 - InP1;

	Normal = v1.Cross(v2).Normalize();
	D = -Normal.Dot(InP1);
}

Plane Plane::Normalize()
{
	float size = sqrtf(Normal.SizeSquared());
	return Plane(Normal /= size, D /= size);
}
