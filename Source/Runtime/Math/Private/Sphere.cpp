#include "Precompiled.h"
#include "Sphere.h"

void Sphere::CalcSphere(Vector4 * InVertices, int VertexCount)
{
	Vector4 avgPos;
	for (int vi = 0; vi < VertexCount; ++vi)
	{
		avgPos += InVertices[vi];
	}

	avgPos /= VertexCount;
	Center = avgPos.ToVector3();

	float maxSize = 0.f;
	for (int vi = 0; vi < VertexCount; ++vi)
	{
		float sizeSquared = (avgPos - InVertices[vi]).SizeSquared();
		if (sizeSquared > maxSize)
		{
			maxSize = sizeSquared;
		}
	}

	Radius = sqrtf(maxSize);
}
