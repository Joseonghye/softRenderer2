#pragma once
#include "Vector4.h"

class Mesh
{
public:
	void SetMesh (int InVertexCount, Vector4* InV,int InIndexCount, int* InI)
	{
		VertexCount = InVertexCount;
		Vertices = new Vector4[VertexCount];
		Vertices = InV;

		IndexCount = InIndexCount;
		Indices = new int[IndexCount];
		Indices = InI;
	}

	int GetVertexCount() const { return VertexCount; }
	int GetIndexCount() const { return IndexCount; }
	int GetTriangleSize() { return IndexCount / 3; }
	Vector4* GetVertexBuffer() { return Vertices; }
	int* GetIndexBuffer() { return Indices; }
	
	int VertexCount;
	int IndexCount;

	Vector4* Vertices;
	int* Indices;
};
