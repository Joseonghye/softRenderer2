
#include "Precompiled.h"
#include "Mesh.h"

void Mesh::SetMesh(int InVertexCount, int InIndexCount, Vector4 * InVertexBuffer, int * InIndexBuffer)
{
	VertexCount = InVertexCount;
	IndexCount = InIndexCount;

	Vertices = new Vector4[InVertexCount];
	Indices = new int[InIndexCount];

	for (int vi = 0; vi < InVertexCount; vi++)
	{
		Vertices[vi] = InVertexBuffer[vi];
	}

	for (int i = 0; i < InIndexCount; i++)
	{
		Indices[i] = InIndexBuffer[i];
	}

	BoundingBoxArea.CalcBoundingBox(Vertices, VertexCount);
	SphereArea.CalcSphere(Vertices, VertexCount);
}

void Mesh::SetColor(Color32 * InColorBuffer)
{
	Colors = InColorBuffer;
}

void Mesh::ReleaseAllBuffers()
{
	if (nullptr != Vertices)
	{
		delete[] Vertices;
		Vertices = nullptr;
	}

	if (nullptr != Indices)
	{
		delete[] Indices;
		Indices = nullptr;
	}
}
