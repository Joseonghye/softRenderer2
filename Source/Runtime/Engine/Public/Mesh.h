#pragma once

#include <list>
#include "Vector4.h"

using namespace std;

class Mesh
{
public:
	void SetVertex (int vc, Vector4* v ) 
	{
		VertexCount = vc;
		Verties = new Vector4[vc];
		Verties = v;
	}
	void SetIndex(int tc, int* i)
	{
		TriangleCount = tc;
		IndexCount = tc * 3;
		Indices = new int[IndexCount];
		Indices = i;
	}

	int GetVertexCount() { return VertexCount; }
	int GetTriangleSize() { return TriangleCount; }

	int VertexCount;
	int IndexCount;
	int TriangleCount;

	Vector4* Verties;
	int* Indices;
};
