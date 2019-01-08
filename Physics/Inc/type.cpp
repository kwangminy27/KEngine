#include "Physics.h"
#include "type.h"

K::Vector2 const K::Vector2::Zero{ 0.f, 0.f };
K::Vector2 const K::Vector2::One{ 1.f, 1.f };
K::Vector2 const K::Vector2::UnitX{ 1.f, 0.f };
K::Vector2 const K::Vector2::UnitY{ 0.f, 1.f };

K::Vector3 const K::Vector3::Zero{ 0.f, 0.f, 0.f };
K::Vector3 const K::Vector3::One{ 1.f, 1.f, 1.f };
K::Vector3 const K::Vector3::UnitX{ 1.f, 0.f, 0.f };
K::Vector3 const K::Vector3::UnitY{ 0.f, 1.f, 0.f };
K::Vector3 const K::Vector3::UnitZ{ 0.f, 0.f, 1.f };

K::Vector4 const K::Vector4::Zero{ 0.f, 0.f, 0.f, 0.f };
K::Vector4 const K::Vector4::One{ 1.f, 1.f, 1.f, 1.f };
K::Vector4 const K::Vector4::UnitX{ 1.f, 0.f, 0.f, 0.f };
K::Vector4 const K::Vector4::UnitY{ 0.f, 1.f, 0.f, 0.f };
K::Vector4 const K::Vector4::UnitZ{ 0.f, 0.f, 1.f, 0.f };
K::Vector4 const K::Vector4::UnitW{ 0.f, 0.f, 0.f, 1.f };

K::Quaternion const K::Quaternion::Identity{ 0.f, 0.f, 0.f, 1.f };

K::Matrix const K::Matrix::Identity{
	1.f, 0.f, 0.f, 0.f,
	0.f, 1.f, 0.f, 0.f,
	0.f, 0.f, 1.f, 0.f,
	0.f, 0.f, 0.f, 1.f
};

#pragma region Vector2
inline K::Vector2 K::Vector2::operator+(Vector2 const& _v) const
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(this));
	XMVECTOR v2 = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(&_v));

	Vector2 R{};
	XMStoreFloat2(reinterpret_cast<XMFLOAT2*>(&R), XMVectorAdd(v1, v2));

	return R;
}

inline K::Vector2 K::Vector2::operator-(Vector2 const& _v) const
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(this));
	XMVECTOR v2 = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(&_v));

	Vector2 R{};
	XMStoreFloat2(reinterpret_cast<XMFLOAT2*>(&R), XMVectorSubtract(v1, v2));

	return R;
}

inline K::Vector2 K::Vector2::operator*(Vector2 const& _v) const
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(this));
	XMVECTOR v2 = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(&_v));

	Vector2 R{};
	XMStoreFloat2(reinterpret_cast<XMFLOAT2*>(&R), XMVectorMultiply(v1, v2));

	return R;
}

inline K::Vector2 K::Vector2::operator*(float _s) const
{
	using namespace DirectX;

	XMVECTOR v = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(this));

	Vector2 R{};
	XMStoreFloat2(reinterpret_cast<XMFLOAT2*>(&R), XMVectorScale(v, _s));

	return R;
}

inline K::Vector2 K::Vector2::operator/(Vector2 const& _v) const
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(this));
	XMVECTOR v2 = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(&_v));

	Vector2 R{};
	XMStoreFloat2(reinterpret_cast<XMFLOAT2*>(&R), XMVectorDivide(v1, v2));

	return R;
}

inline K::Vector2 K::Vector2::operator/(float _s) const
{
	assert(_s != 0.f);

	using namespace DirectX;

	XMVECTOR v = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(this));

	Vector2 R{};
	XMStoreFloat2(reinterpret_cast<XMFLOAT2*>(&R), XMVectorScale(v, 1.f / _s));

	return R;
}

inline bool K::Vector2::operator==(Vector2 const& _v) const
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(this));
	XMVECTOR v2 = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(&_v));

	return XMVector2Equal(v1, v2);
}

inline bool K::Vector2::operator!=(Vector2 const& _v) const
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(this));
	XMVECTOR v2 = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(&_v));

	return XMVector2NotEqual(v1, v2);
}

inline K::Vector2& K::Vector2::operator+=(Vector2 const& _v)
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(this));
	XMVECTOR v2 = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(&_v));

	XMStoreFloat2(reinterpret_cast<XMFLOAT2*>(this), XMVectorAdd(v1, v2));

	return *this;
}

inline K::Vector2& K::Vector2::operator-=(Vector2 const& _v)
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(this));
	XMVECTOR v2 = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(&_v));

	XMStoreFloat2(reinterpret_cast<XMFLOAT2*>(this), XMVectorSubtract(v1, v2));

	return *this;
}

inline K::Vector2& K::Vector2::operator*=(Vector2 const& _v)
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(this));
	XMVECTOR v2 = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(&_v));

	XMStoreFloat2(reinterpret_cast<XMFLOAT2*>(this), XMVectorMultiply(v1, v2));

	return *this;
}

inline K::Vector2& K::Vector2::operator*=(float _s)
{
	using namespace DirectX;

	XMVECTOR v = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(this));

	XMStoreFloat2(reinterpret_cast<XMFLOAT2*>(this), XMVectorScale(v, _s));

	return *this;
}

inline K::Vector2& K::Vector2::operator/=(Vector2 const& _v)
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(this));
	XMVECTOR v2 = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(&_v));

	XMStoreFloat2(reinterpret_cast<XMFLOAT2*>(this), XMVectorDivide(v1, v2));

	return *this;
}

inline K::Vector2& K::Vector2::operator/=(float _s)
{
	assert(_s != 0.f);

	using namespace DirectX;

	XMVECTOR v = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(this));

	XMStoreFloat2(reinterpret_cast<XMFLOAT2*>(this), XMVectorScale(v, 1.f / _s));

	return *this;
}

inline float K::Vector2::Length() const
{
	using namespace DirectX;

	XMVECTOR v = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(this));

	return XMVectorGetX(XMVector2Length(v));
}

inline float K::Vector2::LengthSquared() const
{
	using namespace DirectX;

	XMVECTOR v = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(this));

	return XMVectorGetX(XMVector2LengthSq(v));
}

inline float K::Vector2::Dot(Vector2 const& _v) const
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(this));
	XMVECTOR v2 = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(&_v));

	return XMVectorGetX(XMVector2Dot(v1, v2));
}

inline K::Vector2 K::Vector2::Cross(Vector2 const& _v) const
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(this));
	XMVECTOR v2 = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(&_v));

	Vector2 R{};
	XMStoreFloat2(reinterpret_cast<XMFLOAT2*>(&R), XMVector2Cross(v1, v2));

	return R;
}

inline void K::Vector2::Normalize()
{
	using namespace DirectX;

	XMVECTOR v = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(this));

	XMStoreFloat2(reinterpret_cast<XMFLOAT2*>(this), XMVector2Normalize(v));
}

inline void K::Vector2::Clamp(Vector2 const& _min, Vector2 const& _max)
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(this));
	XMVECTOR v2 = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(&_min));
	XMVECTOR v3 = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(&_max));

	XMStoreFloat2(reinterpret_cast<XMFLOAT2*>(this), XMVectorClamp(v1, v2, v3));
}

inline float K::Vector2::Distance(Vector2 const& _v1, Vector2 const& _v2)
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(&_v1));
	XMVECTOR v2 = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(&_v2));

	return XMVectorGetX(XMVector2Length(XMVectorSubtract(v2, v1)));
}

inline float K::Vector2::DistanceSquared(Vector2 const& _v1, Vector2 const& _v2)
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(&_v1));
	XMVECTOR v2 = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(&_v2));

	return XMVectorGetX(XMVector2LengthSq(XMVectorSubtract(v2, v1)));
}

inline K::Vector2 K::Vector2::Min(Vector2 const& _v1, Vector2 const& _v2)
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(&_v1));
	XMVECTOR v2 = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(&_v2));

	Vector2 R{};
	XMStoreFloat2(reinterpret_cast<XMFLOAT2*>(&R), XMVectorMin(v1, v2));

	return R;
}

inline K::Vector2 K::Vector2::Max(Vector2 const& _v1, Vector2 const& _v2)
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(&_v1));
	XMVECTOR v2 = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(&_v2));

	Vector2 R{};
	XMStoreFloat2(reinterpret_cast<XMFLOAT2*>(&R), XMVectorMax(v1, v2));

	return R;
}

inline K::Vector2 K::Vector2::Transform(Vector2 const& _v, Matrix const& _m)
{
	using namespace DirectX;

	XMVECTOR v = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(&_v));
	XMMATRIX m = XMLoadFloat4x4(reinterpret_cast<XMFLOAT4X4 const*>(&_m));

	Vector2 R{};
	XMStoreFloat2(reinterpret_cast<XMFLOAT2*>(&R), XMVector2TransformCoord(v, m));

	return R;
}

inline K::Vector2 K::Vector2::TransformNormal(Vector2 const& _v, Matrix const& _m)
{
	using namespace DirectX;

	XMVECTOR v = XMLoadFloat2(reinterpret_cast<XMFLOAT2 const*>(&_v));
	XMMATRIX m = XMLoadFloat4x4(reinterpret_cast<XMFLOAT4X4 const*>(&_m));

	Vector2 R{};
	XMStoreFloat2(reinterpret_cast<XMFLOAT2*>(&R), XMVector2TransformNormal(v, m));

	return R;
}
#pragma endregion

#pragma region Vector3
inline K::Vector3 K::Vector3::operator+(Vector3 const& _v) const
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(this));
	XMVECTOR v2 = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(&_v));

	Vector3 R{};
	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&R), XMVectorAdd(v1, v2));

	return R;
}

inline K::Vector3 K::Vector3::operator-(Vector3 const& _v) const
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(this));
	XMVECTOR v2 = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(&_v));

	Vector3 R{};
	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&R), XMVectorSubtract(v1, v2));

	return R;
}

inline K::Vector3 K::Vector3::operator*(Vector3 const& _v) const
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(this));
	XMVECTOR v2 = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(&_v));

	Vector3 R{};
	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&R), XMVectorMultiply(v1, v2));

	return R;
}

inline K::Vector3 K::Vector3::operator*(float _s) const
{
	using namespace DirectX;

	XMVECTOR v = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(this));

	Vector3 R{};
	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&R), XMVectorScale(v, _s));

	return R;
}

inline K::Vector3 K::Vector3::operator/(Vector3 const& _v) const
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(this));
	XMVECTOR v2 = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(&_v));

	Vector3 R{};
	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&R), XMVectorDivide(v1, v2));

	return R;
}

inline K::Vector3 K::Vector3::operator/(float _s) const
{
	assert(_s != 0.f);

	using namespace DirectX;

	XMVECTOR v = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(this));

	Vector3 R{};
	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&R), XMVectorScale(v, 1.f / _s));

	return R;
}

inline bool K::Vector3::operator==(Vector3 const& _v) const
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(this));
	XMVECTOR v2 = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(&_v));

	return XMVector3Equal(v1, v2);
}

inline bool K::Vector3::operator!=(Vector3 const& _v) const
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(this));
	XMVECTOR v2 = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(&_v));

	return XMVector3NotEqual(v1, v2);
}

inline K::Vector3& K::Vector3::operator+=(Vector3 const& _v)
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(this));
	XMVECTOR v2 = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(&_v));

	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(this), XMVectorAdd(v1, v2));

	return *this;
}

inline K::Vector3& K::Vector3::operator-=(Vector3 const& _v)
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(this));
	XMVECTOR v2 = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(&_v));

	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(this), XMVectorSubtract(v1, v2));

	return *this;
}

inline K::Vector3& K::Vector3::operator*=(Vector3 const& _v)
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(this));
	XMVECTOR v2 = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(&_v));

	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(this), XMVectorMultiply(v1, v2));

	return *this;
}

inline K::Vector3& K::Vector3::operator*=(float _s)
{
	using namespace DirectX;

	XMVECTOR v = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(this));

	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(this), XMVectorScale(v, _s));

	return *this;
}

inline K::Vector3& K::Vector3::operator/=(Vector3 const& _v)
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(this));
	XMVECTOR v2 = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(&_v));

	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(this), XMVectorDivide(v1, v2));

	return *this;
}

inline K::Vector3& K::Vector3::operator/=(float _s)
{
	assert(_s != 0.f);

	using namespace DirectX;

	XMVECTOR v = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(this));

	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(this), XMVectorScale(v, 1.f / _s));

	return *this;
}

inline float K::Vector3::Length() const
{
	using namespace DirectX;

	XMVECTOR v = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(this));

	return XMVectorGetX(XMVector3Length(v));
}

inline float K::Vector3::LengthSquared() const
{
	using namespace DirectX;

	XMVECTOR v = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(this));

	return XMVectorGetX(XMVector3LengthSq(v));
}

inline float K::Vector3::Dot(Vector3 const& _v) const
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(this));
	XMVECTOR v2 = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(&_v));

	return XMVectorGetX(XMVector3Dot(v1, v2));
}

inline K::Vector3 K::Vector3::Cross(Vector3 const& _v) const
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(this));
	XMVECTOR v2 = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(&_v));

	Vector3 R{};
	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&R), XMVector3Cross(v1, v2));

	return R;
}

inline void K::Vector3::Normalize()
{
	using namespace DirectX;

	XMVECTOR v = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(this));

	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(this), XMVector3Normalize(v));
}

inline void K::Vector3::Clamp(Vector3 const& _min, Vector3 const& _max)
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(this));
	XMVECTOR v2 = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(&_min));
	XMVECTOR v3 = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(&_max));

	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(this), XMVectorClamp(v1, v2, v3));
}

inline float K::Vector3::Distance(Vector3 const& _v1, Vector3 const& _v2)
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(&_v1));
	XMVECTOR v2 = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(&_v2));

	return XMVectorGetX(XMVector3Length(XMVectorSubtract(v2, v1)));
}

inline float K::Vector3::DistanceSquared(Vector3 const& _v1, Vector3 const& _v2)
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(&_v1));
	XMVECTOR v2 = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(&_v2));

	return XMVectorGetX(XMVector3LengthSq(XMVectorSubtract(v2, v1)));
}

inline K::Vector3 K::Vector3::Min(Vector3 const& _v1, Vector3 const& _v2)
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(&_v1));
	XMVECTOR v2 = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(&_v2));

	Vector3 R{};
	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&R), XMVectorMin(v1, v2));

	return R;
}

inline K::Vector3 K::Vector3::Max(Vector3 const& _v1, Vector3 const& _v2)
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(&_v1));
	XMVECTOR v2 = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(&_v2));

	Vector3 R{};
	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&R), XMVectorMax(v1, v2));

	return R;
}

inline K::Vector3 K::Vector3::Transform(Vector3 const& _v, Matrix const& _m)
{
	using namespace DirectX;

	XMVECTOR v = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(&_v));
	XMMATRIX m = XMLoadFloat4x4(reinterpret_cast<XMFLOAT4X4 const*>(&_m));

	Vector3 R{};
	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&R), XMVector3TransformCoord(v, m));

	return R;
}

inline K::Vector3 K::Vector3::TransformNormal(Vector3 const& _v, Matrix const& _m)
{
	using namespace DirectX;

	XMVECTOR v = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(&_v));
	XMMATRIX m = XMLoadFloat4x4(reinterpret_cast<XMFLOAT4X4 const*>(&_m));

	Vector3 R{};
	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&R), XMVector3TransformNormal(v, m));

	return R;
}
#pragma endregion

#pragma region Vector4
inline K::Vector4 K::Vector4::operator+(Vector4 const& _v) const
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(this));
	XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_v));

	Vector4 R{};
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R), XMVectorAdd(v1, v2));

	return R;
}

inline K::Vector4 K::Vector4::operator-(Vector4 const& _v) const
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(this));
	XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_v));

	Vector4 R{};
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R), XMVectorSubtract(v1, v2));

	return R;
}

inline K::Vector4 K::Vector4::operator*(Vector4 const& _v) const
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(this));
	XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_v));

	Vector4 R{};
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R), XMVectorMultiply(v1, v2));

	return R;
}

inline K::Vector4 K::Vector4::operator*(float _s) const
{
	using namespace DirectX;

	XMVECTOR v = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(this));

	Vector4 R{};
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R), XMVectorScale(v, _s));

	return R;
}

inline K::Vector4 K::Vector4::operator/(Vector4 const& _v) const
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(this));
	XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_v));

	Vector4 R{};
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R), XMVectorDivide(v1, v2));

	return R;
}

inline K::Vector4 K::Vector4::operator/(float _s) const
{
	assert(_s != 0.f);

	using namespace DirectX;

	XMVECTOR v = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(this));

	Vector4 R{};
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R), XMVectorScale(v, 1.f / _s));

	return R;
}

inline bool K::Vector4::operator==(Vector4 const& _v) const
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(this));
	XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_v));

	return XMVector4Equal(v1, v2);
}

inline bool K::Vector4::operator!=(Vector4 const& _v) const
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(this));
	XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_v));

	return XMVector4NotEqual(v1, v2);
}

inline K::Vector4& K::Vector4::operator+=(Vector4 const& _v)
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(this));
	XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_v));

	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(this), XMVectorAdd(v1, v2));

	return *this;
}

inline K::Vector4& K::Vector4::operator-=(Vector4 const& _v)
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(this));
	XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_v));

	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(this), XMVectorSubtract(v1, v2));

	return *this;
}

inline K::Vector4& K::Vector4::operator*=(Vector4 const& _v)
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(this));
	XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_v));

	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(this), XMVectorMultiply(v1, v2));

	return *this;
}

inline K::Vector4& K::Vector4::operator*=(float _s)
{
	using namespace DirectX;

	XMVECTOR v = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(this));

	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(this), XMVectorScale(v, _s));

	return *this;
}

inline K::Vector4& K::Vector4::operator/=(Vector4 const& _v)
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(this));
	XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_v));

	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(this), XMVectorDivide(v1, v2));

	return *this;
}

inline K::Vector4& K::Vector4::operator/=(float _s)
{
	assert(_s != 0.f);

	using namespace DirectX;

	XMVECTOR v = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(this));

	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(this), XMVectorScale(v, 1.f / _s));

	return *this;
}

inline float K::Vector4::Length() const
{
	using namespace DirectX;

	XMVECTOR v = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(this));

	return XMVectorGetX(XMVector4Length(v));
}

inline float K::Vector4::LengthSquared() const
{
	using namespace DirectX;

	XMVECTOR v = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(this));

	return XMVectorGetX(XMVector4LengthSq(v));
}

inline float K::Vector4::Dot(Vector4 const& _v) const
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(this));
	XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_v));

	return XMVectorGetX(XMVector4Dot(v1, v2));
}

inline K::Vector4 K::Vector4::Cross(Vector4 const& _v1, Vector4 const& _v2) const
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(this));
	XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_v1));
	XMVECTOR v3 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_v2));

	Vector4 R{};
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R), XMVector4Cross(v1, v2, v3));

	return R;
}

inline void K::Vector4::Normalize()
{
	using namespace DirectX;

	XMVECTOR v = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(this));

	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(this), XMVector4Normalize(v));
}

inline void K::Vector4::Clamp(Vector4 const& _min, Vector4 const& _max)
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(this));
	XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_min));
	XMVECTOR v3 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_max));

	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(this), XMVectorClamp(v1, v2, v3));
}

inline float K::Vector4::Distance(Vector4 const& _v1, Vector4 const& _v2)
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_v1));
	XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_v2));

	return XMVectorGetX(XMVector4Length(XMVectorSubtract(v2, v1)));
}

inline float K::Vector4::DistanceSquared(Vector4 const& _v1, Vector4 const& _v2)
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_v1));
	XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_v2));

	return XMVectorGetX(XMVector4LengthSq(XMVectorSubtract(v2, v1)));
}

inline K::Vector4 K::Vector4::Min(Vector4 const& _v1, Vector4 const& _v2)
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_v1));
	XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_v2));

	Vector4 R{};
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R), XMVectorMin(v1, v2));

	return R;
}

inline K::Vector4 K::Vector4::Max(Vector4 const& _v1, Vector4 const& _v2)
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_v1));
	XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_v2));

	Vector4 R{};
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R), XMVectorMax(v1, v2));

	return R;
}

inline K::Vector4 K::Vector4::Transform(Vector4 const& _v, Matrix const& _m)
{
	using namespace DirectX;

	XMVECTOR v = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_v));
	XMMATRIX m = XMLoadFloat4x4(reinterpret_cast<XMFLOAT4X4 const*>(&_m));

	Vector4 R{};
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R), XMVector4Transform(v, m));

	return R;
}
#pragma endregion

#pragma region Quaternion
inline K::Quaternion K::Quaternion::operator+(Quaternion const& _q) const
{
	using namespace DirectX;

	XMVECTOR q1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(this));
	XMVECTOR q2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_q));

	Quaternion R{};
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R), XMVectorAdd(q1, q2));

	return R;
}

inline K::Quaternion K::Quaternion::operator-(Quaternion const& _q) const
{
	using namespace DirectX;

	XMVECTOR q1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(this));
	XMVECTOR q2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_q));

	Quaternion R{};
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R), XMVectorSubtract(q1, q2));

	return R;
}

inline K::Quaternion K::Quaternion::operator*(Quaternion const& _q) const
{
	using namespace DirectX;

	XMVECTOR q1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(this));
	XMVECTOR q2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_q));

	Quaternion R{};
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R), XMQuaternionMultiply(q1, q2));

	return R;
}

inline K::Quaternion K::Quaternion::operator*(float _s) const
{
	using namespace DirectX;

	XMVECTOR q = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(this));

	Quaternion R{};
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R), XMVectorScale(q, _s));

	return R;
}

inline K::Quaternion K::Quaternion::operator/(Quaternion const& _q) const
{
	using namespace DirectX;

	XMVECTOR q1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(this));
	XMVECTOR q2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_q));
	q2 = XMQuaternionInverse(q2);

	Quaternion R{};
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R), XMQuaternionMultiply(q1, q2));

	return R;
}

inline K::Quaternion K::Quaternion::operator/(float _s) const
{
	assert(_s != 0.f);

	using namespace DirectX;

	XMVECTOR q = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(this));

	Quaternion R{};
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R), XMVectorScale(q, 1.f / _s));

	return R;
}

inline bool K::Quaternion::operator==(Quaternion const& _q) const
{
	using namespace DirectX;

	XMVECTOR q1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(this));
	XMVECTOR q2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_q));

	return XMQuaternionEqual(q1, q2);
}

inline bool K::Quaternion::operator!=(Quaternion const& _q) const
{
	using namespace DirectX;

	XMVECTOR q1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(this));
	XMVECTOR q2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_q));

	return XMQuaternionNotEqual(q1, q2);
}

inline K::Quaternion& K::Quaternion::operator+=(Quaternion const& _q)
{
	using namespace DirectX;

	XMVECTOR q1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(this));
	XMVECTOR q2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_q));

	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(this), XMVectorAdd(q1, q2));

	return *this;
}

inline K::Quaternion& K::Quaternion::operator-=(Quaternion const& _q)
{
	using namespace DirectX;

	XMVECTOR q1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(this));
	XMVECTOR q2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_q));

	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(this), XMVectorSubtract(q1, q2));

	return *this;
}

inline K::Quaternion& K::Quaternion::operator*=(Quaternion const& _q)
{
	using namespace DirectX;

	XMVECTOR q1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(this));
	XMVECTOR q2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_q));

	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(this), XMQuaternionMultiply(q1, q2));

	return *this;
}

inline K::Quaternion& K::Quaternion::operator*=(float _s)
{
	using namespace DirectX;

	XMVECTOR q = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(this));

	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(this), XMVectorScale(q, _s));

	return *this;
}

inline K::Quaternion& K::Quaternion::operator/=(Quaternion const& _q)
{
	using namespace DirectX;

	XMVECTOR q1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(this));
	XMVECTOR q2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_q));
	q2 = XMQuaternionInverse(q2);

	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(this), XMQuaternionMultiply(q1, q2));

	return *this;
}

inline K::Quaternion& K::Quaternion::operator/=(float _s)
{
	assert(_s != 0.f);

	using namespace DirectX;

	XMVECTOR q = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(this));

	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(this), XMVectorScale(q, 1.f / _s));

	return *this;
}

inline K::Quaternion K::Quaternion::CreateFromAxisAngle(Vector3 const& _axis, float _angle)
{
	using namespace DirectX;

	XMVECTOR v = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(&_axis));

	Quaternion R{};
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R), XMQuaternionRotationAxis(v, XMConvertToRadians(_angle)));

	return R;
}

inline K::Quaternion K::Quaternion::CreateFromYawPitchRoll(float _yaw, float _pitch, float _roll)
{
	using namespace DirectX;

	Quaternion R{};
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R), XMQuaternionRotationRollPitchYaw(XMConvertToRadians(_pitch), XMConvertToRadians(_yaw), XMConvertToRadians(_roll)));

	return R;
}

inline K::Quaternion K::Quaternion::CreateFromRotationMatrix(Matrix const& _m)
{
	using namespace DirectX;

	XMMATRIX m = XMLoadFloat4x4(reinterpret_cast<XMFLOAT4X4 const*>(&_m));

	Quaternion R{};
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R), XMQuaternionRotationMatrix(m));

	return R;
}

inline K::Quaternion K::Quaternion::Slerp(Quaternion const& _q1, Quaternion const& _q2, float _t)
{
	using namespace DirectX;

	XMVECTOR q1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_q1));
	XMVECTOR q2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_q2));

	Quaternion R{};
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R), XMQuaternionSlerp(q1, q2, _t));

	return R;
}
#pragma endregion

#pragma region Matrix
inline K::Matrix K::Matrix::operator+(Matrix const& _m) const
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_11));
	XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_21));
	XMVECTOR v3 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_31));
	XMVECTOR v4 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_41));
	XMVECTOR v5 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_m._11));
	XMVECTOR v6 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_m._21));
	XMVECTOR v7 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_m._31));
	XMVECTOR v8 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_m._41));

	Matrix R{};
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), XMVectorAdd(v1, v5));
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), XMVectorAdd(v2, v6));
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), XMVectorAdd(v3, v7));
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), XMVectorAdd(v4, v8));

	return R;
}

inline K::Matrix K::Matrix::operator-(Matrix const& _m) const
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_11));
	XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_21));
	XMVECTOR v3 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_31));
	XMVECTOR v4 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_41));
	XMVECTOR v5 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_m._11));
	XMVECTOR v6 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_m._21));
	XMVECTOR v7 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_m._31));
	XMVECTOR v8 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_m._41));

	Matrix R{};
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), XMVectorSubtract(v1, v5));
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), XMVectorSubtract(v2, v6));
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), XMVectorSubtract(v3, v7));
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), XMVectorSubtract(v4, v8));

	return R;
}

inline K::Matrix K::Matrix::operator*(Matrix const& _m) const
{
	using namespace DirectX;

	XMMATRIX m1 = XMLoadFloat4x4(reinterpret_cast<XMFLOAT4X4 const*>(this));
	XMMATRIX m2 = XMLoadFloat4x4(reinterpret_cast<XMFLOAT4X4 const*>(&_m));

	Matrix R{};
	XMStoreFloat4x4(reinterpret_cast<XMFLOAT4X4*>(&R), XMMatrixMultiply(m1, m2));

	return R;
}

inline K::Matrix K::Matrix::operator*(float _s) const
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_11));
	XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_21));
	XMVECTOR v3 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_31));
	XMVECTOR v4 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_41));

	Matrix R{};
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), XMVectorScale(v1, _s));
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), XMVectorScale(v2, _s));
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), XMVectorScale(v3, _s));
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), XMVectorScale(v4, _s));

	return R;
}

inline K::Matrix K::Matrix::operator/(Matrix const& _m) const
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_11));
	XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_21));
	XMVECTOR v3 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_31));
	XMVECTOR v4 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_41));
	XMVECTOR v5 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_m._11));
	XMVECTOR v6 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_m._21));
	XMVECTOR v7 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_m._31));
	XMVECTOR v8 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_m._41));

	Matrix R{};
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), XMVectorDivide(v1, v5));
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), XMVectorDivide(v2, v6));
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), XMVectorDivide(v3, v7));
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), XMVectorDivide(v4, v8));

	return R;
}

inline K::Matrix K::Matrix::operator/(float _s) const
{
	assert(_s != 0.f);

	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_11));
	XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_21));
	XMVECTOR v3 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_31));
	XMVECTOR v4 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_41));
	_s = 1.f / _s;

	Matrix R{};
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), XMVectorScale(v1, _s));
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), XMVectorScale(v2, _s));
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), XMVectorScale(v3, _s));
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), XMVectorScale(v4, _s));

	return R;
}

inline bool K::Matrix::operator==(Matrix const& _m) const
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_11));
	XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_21));
	XMVECTOR v3 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_31));
	XMVECTOR v4 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_41));
	XMVECTOR v5 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_m._11));
	XMVECTOR v6 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_m._21));
	XMVECTOR v7 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_m._31));
	XMVECTOR v8 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_m._41));

	return XMVector4Equal(v1, v5) && XMVector4Equal(v2, v6) && XMVector4Equal(v3, v7) && XMVector4Equal(v4, v8);
}

inline bool K::Matrix::operator!=(Matrix const& _m) const
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_11));
	XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_21));
	XMVECTOR v3 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_31));
	XMVECTOR v4 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_41));
	XMVECTOR v5 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_m._11));
	XMVECTOR v6 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_m._21));
	XMVECTOR v7 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_m._31));
	XMVECTOR v8 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_m._41));

	return !(XMVector4Equal(v1, v5) && XMVector4Equal(v2, v6) && XMVector4Equal(v3, v7) && XMVector4Equal(v4, v8));
}

inline K::Matrix& K::Matrix::operator+=(Matrix const& _m)
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_11));
	XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_21));
	XMVECTOR v3 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_31));
	XMVECTOR v4 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_41));
	XMVECTOR v5 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_m._11));
	XMVECTOR v6 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_m._21));
	XMVECTOR v7 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_m._31));
	XMVECTOR v8 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_m._41));

	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), XMVectorAdd(v1, v5));
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), XMVectorAdd(v2, v6));
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), XMVectorAdd(v3, v7));
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), XMVectorAdd(v4, v8));

	return *this;
}

inline K::Matrix& K::Matrix::operator-=(Matrix const& _m)
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_11));
	XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_21));
	XMVECTOR v3 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_31));
	XMVECTOR v4 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_41));
	XMVECTOR v5 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_m._11));
	XMVECTOR v6 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_m._21));
	XMVECTOR v7 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_m._31));
	XMVECTOR v8 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_m._41));

	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), XMVectorSubtract(v1, v5));
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), XMVectorSubtract(v2, v6));
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), XMVectorSubtract(v3, v7));
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), XMVectorSubtract(v4, v8));

	return *this;
}

inline K::Matrix& K::Matrix::operator*=(Matrix const& _m)
{
	using namespace DirectX;

	XMMATRIX m1 = XMLoadFloat4x4(reinterpret_cast<XMFLOAT4X4 const*>(this));
	XMMATRIX m2 = XMLoadFloat4x4(reinterpret_cast<XMFLOAT4X4 const*>(&_m));

	XMStoreFloat4x4(reinterpret_cast<XMFLOAT4X4*>(this), XMMatrixMultiply(m1, m2));

	return *this;
}

inline K::Matrix& K::Matrix::operator*=(float _s)
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_11));
	XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_21));
	XMVECTOR v3 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_31));
	XMVECTOR v4 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_41));

	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), XMVectorScale(v1, _s));
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), XMVectorScale(v2, _s));
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), XMVectorScale(v3, _s));
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), XMVectorScale(v4, _s));

	return *this;
}

inline K::Matrix& K::Matrix::operator/=(Matrix const& _m)
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_11));
	XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_21));
	XMVECTOR v3 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_31));
	XMVECTOR v4 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_41));
	XMVECTOR v5 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_m._11));
	XMVECTOR v6 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_m._21));
	XMVECTOR v7 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_m._31));
	XMVECTOR v8 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_m._41));

	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), XMVectorDivide(v1, v5));
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), XMVectorDivide(v2, v6));
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), XMVectorDivide(v3, v7));
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), XMVectorDivide(v4, v8));

	return *this;
}

inline K::Matrix& K::Matrix::operator/=(float _s)
{
	using namespace DirectX;

	XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_11));
	XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_21));
	XMVECTOR v3 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_31));
	XMVECTOR v4 = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_41));
	_s = 1.f / _s;

	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), XMVectorScale(v1, _s));
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), XMVectorScale(v2, _s));
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), XMVectorScale(v3, _s));
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), XMVectorScale(v4, _s));

	return *this;
}

inline K::Matrix K::Matrix::Transpose() const
{
	using namespace DirectX;

	XMMATRIX m = XMLoadFloat4x4(reinterpret_cast<XMFLOAT4X4 const*>(this));

	Matrix R{};
	XMStoreFloat4x4(reinterpret_cast<XMFLOAT4X4*>(&R), XMMatrixTranspose(m));

	return R;
}

inline K::Matrix K::Matrix::Inverse() const
{
	using namespace DirectX;

	XMMATRIX m = XMLoadFloat4x4(reinterpret_cast<XMFLOAT4X4 const*>(this));

	Matrix R{};
	XMStoreFloat4x4(reinterpret_cast<XMFLOAT4X4*>(&R), XMMatrixInverse(nullptr, m));

	return R;
}

inline K::Matrix K::Matrix::CreateScaling(Vector3 const& _v)
{
	using namespace DirectX;

	Matrix R{};
	XMStoreFloat4x4(reinterpret_cast<XMFLOAT4X4*>(&R), XMMatrixScaling(_v.x, _v.y, _v.x));

	return R;
}

inline K::Matrix K::Matrix::CreateRotationX(float _angle)
{
	using namespace DirectX;

	Matrix R{};
	XMStoreFloat4x4(reinterpret_cast<XMFLOAT4X4*>(&R), XMMatrixRotationX(XMConvertToRadians(_angle)));

	return R;
}

inline K::Matrix K::Matrix::CreateRotationY(float _angle)
{
	using namespace DirectX;

	Matrix R{};
	XMStoreFloat4x4(reinterpret_cast<XMFLOAT4X4*>(&R), XMMatrixRotationY(XMConvertToRadians(_angle)));

	return R;
}

inline K::Matrix K::Matrix::CreateRotationZ(float _angle)
{
	using namespace DirectX;

	Matrix R{};
	XMStoreFloat4x4(reinterpret_cast<XMFLOAT4X4*>(&R), XMMatrixRotationZ(XMConvertToRadians(_angle)));

	return R;
}

inline K::Matrix K::Matrix::CreateFromAxisAngle(Vector3 const& _axis, float _angle)
{
	using namespace DirectX;

	XMVECTOR a = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(&_axis));

	Matrix R{};
	XMStoreFloat4x4(reinterpret_cast<XMFLOAT4X4*>(&R), XMMatrixRotationAxis(a, XMConvertToRadians(_angle)));

	return R;
}

inline K::Matrix K::Matrix::CreateFromYawPitchRoll(float _yaw, float _pitch, float _roll)
{
	using namespace DirectX;

	Matrix R{};
	XMStoreFloat4x4(reinterpret_cast<XMFLOAT4X4*>(&R), XMMatrixRotationRollPitchYaw(_pitch, _yaw, _roll));

	return R;
}

inline K::Matrix K::Matrix::CreateFromQuaternion(Quaternion const& _q)
{
	using namespace DirectX;

	XMVECTOR q = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_q));

	Matrix R{};
	XMStoreFloat4x4(reinterpret_cast<XMFLOAT4X4*>(&R), XMMatrixRotationQuaternion(q));

	return R;
}

inline K::Matrix K::Matrix::CreateTranslation(Vector3 const& _v)
{
	using namespace DirectX;

	Matrix R{};
	XMStoreFloat4x4(reinterpret_cast<XMFLOAT4X4*>(&R), XMMatrixTranslation(_v.x, _v.y, _v.z));

	return R;
}

inline K::Matrix K::Matrix::CreateLookAt(Vector3 const& _eye, Vector3 const& _focus, Vector3 const& _up)
{
	using namespace DirectX;

	XMVECTOR eye = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(&_eye));
	XMVECTOR focus = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(&_focus));
	XMVECTOR up = XMLoadFloat3(reinterpret_cast<XMFLOAT3 const*>(&_up));

	Matrix R{};
	XMStoreFloat4x4(reinterpret_cast<XMFLOAT4X4*>(&R), XMMatrixLookAtLH(eye, focus, up));

	return R;
}

inline K::Matrix K::Matrix::CreatePerspectiveFieldOfView(float _fov_angle, float _aspect_ratio, float _near, float _far)
{
	using namespace DirectX;

	Matrix R{};
	XMStoreFloat4x4(reinterpret_cast<XMFLOAT4X4*>(&R), XMMatrixPerspectiveFovLH(_fov_angle, _aspect_ratio, _near, _far));

	return R;
}

inline K::Matrix K::Matrix::CreatePerspective(float _width, float _height, float _near, float _far)
{
	using namespace DirectX;

	Matrix R{};
	XMStoreFloat4x4(reinterpret_cast<XMFLOAT4X4*>(&R), XMMatrixPerspectiveLH(_width, _height, _near, _far));

	return R;
}

inline K::Matrix K::Matrix::CreatePerspectiveOffCenter(float _left, float _right, float _bottom, float _top, float _near, float _far)
{
	using namespace DirectX;

	Matrix R{};
	XMStoreFloat4x4(reinterpret_cast<XMFLOAT4X4*>(&R), XMMatrixPerspectiveOffCenterLH(_left, _right, _bottom, _top, _near, _far));

	return R;
}

inline K::Matrix K::Matrix::CreateOrthographic(float _width, float _height, float _near, float _far)
{
	using namespace DirectX;

	Matrix R{};
	XMStoreFloat4x4(reinterpret_cast<XMFLOAT4X4*>(&R), XMMatrixOrthographicLH(_width, _height, _near, _far));

	return R;
}

inline K::Matrix K::Matrix::CreateOrthographicOffCenter(float _left, float _right, float _bottom, float _top, float _near, float _far)
{
	using namespace DirectX;

	Matrix R{};
	XMStoreFloat4x4(reinterpret_cast<XMFLOAT4X4*>(&R), XMMatrixOrthographicOffCenterLH(_left, _right, _bottom, _top, _near, _far));

	return R;
}

inline K::Matrix K::Matrix::Transform(Matrix const& _m, Quaternion const& _q)
{
	using namespace DirectX;

	XMVECTOR q = XMLoadFloat4(reinterpret_cast<XMFLOAT4 const*>(&_q));

	XMMATRIX m1 = XMLoadFloat4x4(reinterpret_cast<XMFLOAT4X4 const*>(&_m));
	XMMATRIX m2 = XMMatrixRotationQuaternion(q);

	Matrix R{};
	XMStoreFloat4x4(reinterpret_cast<XMFLOAT4X4*>(&R), XMMatrixMultiply(m1, m2));

	return R;
}
#pragma endregion
