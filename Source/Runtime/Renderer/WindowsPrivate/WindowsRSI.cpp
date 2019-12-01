
#include "Precompiled.h"
#include "WindowsRSI.h"
#include "WindowsGDI.h"

WindowsRSI::~WindowsRSI()
{
}

bool WindowsRSI::Init(const ScreenPoint& InScreenSize)
{
	SetBlendingMode(BlendingMode);
	return InitializeGDI(InScreenSize);
}

void WindowsRSI::Shutdown()
{
	ReleaseGDI();
}

void WindowsRSI::SetBlendingMode(BlendingModes InNewBlendingMode)
{
	BlendingMode = InNewBlendingMode;
	//using namespace std::placeholders;
	//if (BlendingMode == BlendingModes::Opaque)
	//{
	//	SetPixel = std::bind(&WindowsGDI::SetPixelOpaque, this, _1, _2);
	//}
	//else if (BlendingMode == BlendingModes::AlphaBlending)
	//{
	//	SetPixel = std::bind(&WindowsGDI::SetPixelAlphaBlending, this, _1, _2);
	//}
}

void WindowsRSI::Clear(const LinearColor & InClearColor)
{
	FillBuffer(InClearColor.ToColor32());
	ClearDepthBuffer();
}

void WindowsRSI::BeginFrame()
{
}

void WindowsRSI::EndFrame()
{
	SwapBuffer();
}

void WindowsRSI::DrawPoint(const Vector2& InVectorPos, const LinearColor& InColor)
{
	SetPixel(ScreenPoint::ToScreenCoordinate(ScreenSize, InVectorPos), InColor);
}

void WindowsRSI::DrawLine(const Vector2 & InStartPos, const Vector2 & InEndPos, const LinearColor & InColor)
{
	ScreenPoint startPos = ScreenPoint::ToScreenCoordinate(ScreenSize, InStartPos);
	ScreenPoint endPos = ScreenPoint::ToScreenCoordinate(ScreenSize, InEndPos);

	if (startPos.X < 0)
	{
		startPos.X = 0;
	}

	if (startPos.X > ScreenSize.X)
	{
		startPos.X = ScreenSize.X;
	}

	if (startPos.Y < 0)
	{
		startPos.Y = 0;
	}

	if (startPos.Y > ScreenSize.Y)
	{
		startPos.Y = ScreenSize.Y;
	}

	if (endPos.X < 0)
	{
		endPos.X = 0;
	}

	if (endPos.X > ScreenSize.X)
	{
		endPos.X = ScreenSize.X;
	}

	if (endPos.Y < 0)
	{
		endPos.Y = 0;
	}

	if (endPos.Y > ScreenSize.Y)
	{
		endPos.Y = ScreenSize.Y;
	}

	int w = endPos.X - startPos.X;
	int h = endPos.Y - startPos.Y;

	bool slope = (Math::Abs(w) >= Math::Abs(h));
	int dirx = w > 0 ? 1 : -1;
	int diry = h > 0 ? 1 : -1;
	int fw = dirx * w;
	int fh = diry * h;

	int f = slope ? fh * 2 - fw : 2 * fw - fh;
	int f1 = slope ? 2 * fh : 2 * fw;
	int f2 = slope ? 2 * (fh - fw) : 2 * (fw - fh);
	int x = startPos.X;
	int y = startPos.Y;

	if (slope)
	{
		while (x != endPos.X)
		{
			SetPixel(ScreenPoint(x, y), InColor);

			if (f < 0)
			{
				f += f1;
			}
			else
			{
				f += f2;
				y += diry;
			}

			x += dirx;
		}
	}
	else
	{
		while (y != endPos.Y)
		{
			SetPixel(ScreenPoint(x, y), InColor);

			if (f < 0)
			{
				f += f1;
			}
			else
			{
				f += f2;
				x += dirx;
			}

			y += diry;
		}
	}
}

void WindowsRSI::SetUniformMatrix(Matrix4x4 * InMatrixBuffer)
{
	ModelingMatrix = InMatrixBuffer[0];
	ViewMatrix = InMatrixBuffer[1];
	ProjectionMatrix = InMatrixBuffer[2];
}

void WindowsRSI::SetVertexBuffer(VertexData * InVertexBuffer)
{
	VertexBuffer = InVertexBuffer;
}

void WindowsRSI::SetIndexBuffer(int * InIndexBuffer)
{
	IndexBuffer = InIndexBuffer;
}

void WindowsRSI::DrawLinePrimitive(UINT InVertexSize, UINT InIndexSize, float InThickness)
{
	Matrix4x4 FinalMatrix = ProjectionMatrix * ViewMatrix * ModelingMatrix;

	if (InThickness == 1.f)
	{
		int lineCount = InIndexSize / 2;
		for (int l = 0; l < lineCount; l++)
		{
			Vector4 lp[2];
			lp[0] = VertexBuffer[IndexBuffer[l * 2]].Position;
			lp[1] = VertexBuffer[IndexBuffer[l * 2 + 1]].Position;

			LinearColor c = VertexBuffer[IndexBuffer[l * 2]].Color;

			for (int li = 0; li < 2; li++)
			{
				lp[li] = FinalMatrix * lp[li];
				float invW = 1.f / lp[li].W;
				lp[li].X *= invW;
				lp[li].Y *= invW;
				lp[li].Z *= invW;
			}

			// Strech to ScreenSize
			for (int li = 0; li < 2; li++)
			{
				lp[li].X *= (ScreenSize.X * 0.5f);
				lp[li].Y *= (ScreenSize.Y * 0.5f);
			}

			DrawLine(lp[0].ToVector2(), lp[1].ToVector2(), c);
		}
	}
	else
	{
		// 네 점을 구한 후 삼각형 두 개를 그려서 완성하기.

		float halfThickness = InThickness * 0.5f;

		int lineCount = InIndexSize / 2;
		for (int l = 0; l < lineCount; l++)
		{
			Vector4 lp[2];
			lp[0] = VertexBuffer[IndexBuffer[l * 2]].Position;
			lp[1] = VertexBuffer[IndexBuffer[l * 2 + 1]].Position;
			LinearColor c = VertexBuffer[IndexBuffer[l * 2]].Color;

			for (int li = 0; li < 2; li++)
			{
				lp[li] = FinalMatrix * lp[li];
				float invW = 1.f / lp[li].W;
				lp[li].X *= invW;
				lp[li].Y *= invW;
				lp[li].Z *= invW;
			}

			// Strech to ScreenSize
			for (int li = 0; li < 2; li++)
			{
				lp[li].X *= (ScreenSize.X * 0.5f);
				lp[li].Y *= (ScreenSize.Y * 0.5f);
			}

			Vector2 screenLP[2] = { lp[0].ToVector2(), lp[1].ToVector2() };
			Vector2 line = (screenLP[1] - screenLP[0]).Normalize() * halfThickness;
			Vector2 inv90RotLine = Vector2(line.Y, -line.X);
			Vector2 screenQuad[4] = {
				screenLP[0] + inv90RotLine,
				screenLP[0] - inv90RotLine,
				screenLP[1] + inv90RotLine,
				screenLP[1] - inv90RotLine
			};
		}


	}
}

void WindowsRSI::DrawTrianglePrimitive(UINT InVertexSize, UINT InIndexSize)
{
	Matrix4x4 FinalMatrix = ProjectionMatrix * ViewMatrix * ModelingMatrix;

	int triangleCount = InIndexSize / 3;
	for (int t = 0; t < triangleCount; t++)
	{
		Vector4 tp[3];
		tp[0] = VertexBuffer[IndexBuffer[t * 3]].Position;
		tp[1] = VertexBuffer[IndexBuffer[t * 3 + 1]].Position;
		tp[2] = VertexBuffer[IndexBuffer[t * 3 + 2]].Position;

		LinearColor c = VertexBuffer[IndexBuffer[t * 3]].Color;

		for (int ti = 0; ti < 3; ti++)
		{
			tp[ti] = FinalMatrix * tp[ti];
			float invW = 1.f / tp[ti].W;
			tp[ti].X *= invW;
			tp[ti].Y *= invW;
			tp[ti].Z *= invW;
		}

		// Backface Culling
		Vector3 edge1 = (tp[1] - tp[0]).ToVector3();
		Vector3 edge2 = (tp[2] - tp[0]).ToVector3();
		Vector3 faceNormal = edge2.Cross(edge1).Normalize();
		static Vector3 cameraDir = -Vector3::UnitZ;
		if (cameraDir.Dot(faceNormal) < 0.f)
		{
			continue;
		}

		// Strech to ScreenSize
		for (int ti = 0; ti < 3; ti++)
		{
			tp[ti].X *= (ScreenSize.X * 0.5f);
			tp[ti].Y *= (ScreenSize.Y * 0.5f);
		}

		DrawLine(tp[0].ToVector2(), tp[1].ToVector2(), c);
		DrawLine(tp[0].ToVector2(), tp[2].ToVector2(), c);
		DrawLine(tp[1].ToVector2(), tp[2].ToVector2(), c);
	}
}

void WindowsRSI::DrawFullVerticalLine(int InX, const LinearColor & InColor)
{
	if (InX < 0 || InX >= ScreenSize.X)
	{
		return;
	}

	for (int y = 0; y < ScreenSize.Y; ++y)
	{
		SetPixel(ScreenPoint(InX, y), InColor);
	}

	return;
}

void WindowsRSI::DrawFullHorizontalLine(int InY, const LinearColor & InColor)
{
	if (InY < 0 || InY >= ScreenSize.Y)
	{
		return;
	}

	for (int x = 0; x < ScreenSize.X; ++x)
	{
		SetPixel(ScreenPoint(x, InY), InColor);
	}
}

void WindowsRSI::SetPixel(const ScreenPoint& InPos, const LinearColor& InColor)
{
	if (BlendingMode == BlendingModes::Opaque)
	{
		SetPixelOpaque(InPos, InColor);
	}
	else if (BlendingMode == BlendingModes::AlphaBlending)
	{
		SetPixelAlphaBlending(InPos, InColor);
	}
}
