#pragma once

class Transform
{
public:
	Transform() = default;

public:
	void SetPosition(const Vector3& InPosition);
	void AddPosition(const Vector3& InPosition);
	void SetRotation(const Rotator& InRotation);
	void AddYawRotation(float InDegree);
	void AddRollRotation(float InDegree);
	void AddPitchRotation(float InDegree);
	void SetScale(const Vector3& InScale);
	Matrix4x4 GetModelingMatrix() const;
	Vector3 GetPosition() const { return Position; }
	Vector3 GetScale() const { return Scale; }

private:
	void CalcLocalAxis();

	Vector3 Position;
	Rotator Rotation;
	Vector3 Scale = Vector3::One;

	Vector3 Forward = Vector3::UnitZ;
	Vector3 Right = Vector3::UnitX;
	Vector3 Up = Vector3::UnitY;

	friend class GameObject;
	friend class Camera;
};

