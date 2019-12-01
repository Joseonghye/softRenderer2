#pragma once

#include "Vector4.h"

struct BoundingBox
{
public:
	BoundingBox() = default;
	void CalcBoundingBox(Vector4 * InVertices, int VertexCount);

public:
	Vector3 Center = Vector3::Zero;
	Vector3 Extent = Vector3::Zero;
};


