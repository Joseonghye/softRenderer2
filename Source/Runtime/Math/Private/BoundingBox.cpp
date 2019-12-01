#include "Precompiled.h"
#include "MathUtil.h"
#include "BoundingBox.h"

void BoundingBox::CalcBoundingBox(Vector4 * InVertices, int VertexCount)
{
	Vector3 min = Vector3::Infinity;
	Vector3 max = Vector3::InfinityNeg;
	for (int vi = 0; vi < VertexCount; ++vi)
	{
		max.X =  Math::Max(max.X, InVertices[vi].X);
		max.Y = Math::Max(max.Y, InVertices[vi].Y);
		max.Z = Math::Max(max.Z, InVertices[vi].Z);

		min.X = Math::Min(min.X, InVertices[vi].X);
		min.Y = Math::Min(min.Y, InVertices[vi].Y);
		min.Z = Math::Min(min.Z, InVertices[vi].Z);
	}

	Center = (min + max) * 0.5f;
	Extent = (max - min) * 0.5f;
}
