#pragma once

#include "Vector4.h"

struct Sphere
{
public:
	Sphere() = default;
	void CalcSphere(Vector4 * InVertices, int VertexCount);

public:
	Vector3 Center = Vector3::Zero;
	float Radius = 0.f;
};


