
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

void WindowsRSI::SetUniformMatrix(Matrix4x4 * InMatrixData)
{
	UniformMatrix = InMatrixData[0] * InMatrixData[1] * InMatrixData[2];
}

void WindowsRSI::SetVertexBuffer(VertexData * InVertexData)
{
	Vertices = InVertexData;
}

void WindowsRSI::SetIndexBuffer(int * InIndexData)
{
	Indices = InIndexData;
}

void WindowsRSI::DrawPrimitive(UINT InVertexSize, UINT InIndexSize)
{
	for (int t = 0; t < InIndexSize / 3; t++)
	{
		VertexData tp[3];
		tp[0] = Vertices[Indices[t * 3]];
		tp[1] = Vertices[Indices[t * 3 + 1]];
		tp[2] = Vertices[Indices[t * 3 + 2]];

		for (int ti = 0; ti < 3; ti++)
		{
			tp[ti].Position = UniformMatrix * tp[ti].Position;
			float repW = 1.f / tp[ti].Position.W;

			tp[ti].Position.X *= repW;
			tp[ti].Position.Y *= repW;
			tp[ti].Position.Z *= repW;
		}

		//BackFace Culling
		Vector3 edge1 = (tp[1].Position - tp[0].Position).ToVector3();
		Vector3 edge2 = (tp[2].Position - tp[0].Position).ToVector3();

		Vector3 normal = -edge2.Cross(edge1).Normalize();
		Vector3 camera = -Vector3::UnitZ;

		if (camera.Dot(normal) <= 0.f)
		{
			// 종횡비가 다르기에 원래대로 맞춰주기 위해서 
			for (int ti = 0; ti < 3; ti++)
			{
				tp[ti].Position.X *= (ScreenSize.X * 0.5f);
				tp[ti].Position.Y *= (ScreenSize.Y * 0.5f);
			}

			DrawLine(tp[0].Position.ToVector2(), tp[1].Position.ToVector2(), LinearColor::Red);
			DrawLine(tp[0].Position.ToVector2(), tp[2].Position.ToVector2(), LinearColor::Red);
			DrawLine(tp[1].Position.ToVector2(), tp[2].Position.ToVector2(), LinearColor::Red);
		}
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
