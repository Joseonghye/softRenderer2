#pragma once

class Mesh
{
public:
	Mesh() = default;
	~Mesh()
	{
		ReleaseAllBuffers();
	}

	int GetVertexCount() const { return VertexCount; }
	int GetIndexCount() const { return IndexCount; }
	Vector4* GetVertexBuffer() { return Vertices; }
	Color32* GetColorBuffer() { return Colors; }
	int* GetIndexBuffer() { return Indices; }
	void SetMesh(int InVertexCount, int InIndexCount, Vector4* InVertexBuffer, int* InIndexBuffer);
	void SetColor(Color32* InColorBuffer);
	void ReleaseAllBuffers();
	Sphere GetSphereArea() const { return SphereArea; }

private:
	int VertexCount = 0;
	int IndexCount = 0;

	Vector4* Vertices = nullptr;
	Color32* Colors = nullptr;
	int* Indices = nullptr;

	BoundingBox BoundingBoxArea;
	Sphere SphereArea;
	Vector3 Pivot = Vector3::Zero;
};

