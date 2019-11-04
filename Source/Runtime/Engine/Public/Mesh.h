#pragma once

#include <list>
#include "Vector4.h"

using namespace std;

class Mesh
{
	int GetVertexCount() { return Verteies.size(); }
	int GetTriangleSize() { return Indices.size() / 3.f; }

	list<Vector4> Verteies;
	list<int> Indices;
};
