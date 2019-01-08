#pragma once

namespace K
{
	struct Vector2;
	struct Vector3;
	struct Vector4;
	struct Quaternion;
	struct Matrix;

	struct PHYSICS_DLL Vector2
	{
		Vector2() = default;
		Vector2(Vector2 const&) = default;
		Vector2(Vector2&&) noexcept = default;
		Vector2(float _x, float _y) : x(_x), y(_y) {}
		Vector2(DirectX::XMFLOAT2 const& _v) { memcpy_s(this, sizeof(Vector2), &_v, sizeof(Vector2)); }

		Vector2(DirectX::FXMVECTOR _v) { DirectX::XMStoreFloat2(reinterpret_cast<DirectX::XMFLOAT2*>(this), _v); }
		operator DirectX::XMVECTOR() const { return DirectX::XMLoadFloat2(reinterpret_cast<DirectX::XMFLOAT2 const*>(this)); }

		Vector2 operator+() const { return *this; }
		Vector2 operator-() const { return Vector2(-x, -y); }

		Vector2 operator+(Vector2 const& _v) const;
		Vector2 operator-(Vector2 const& _v) const;
		Vector2 operator*(Vector2 const& _v) const;
		Vector2 operator*(float _s) const;
		Vector2 operator/(Vector2 const& _v) const;
		Vector2 operator/(float _s) const;

		bool operator==(Vector2 const& _v) const;
		bool operator!=(Vector2 const& _v) const;

		Vector2& operator=(Vector2 const&) = default;
		Vector2& operator=(Vector2&&) noexcept = default;
		Vector2& operator+=(Vector2 const& _v);
		Vector2& operator-=(Vector2 const& _v);
		Vector2& operator*=(Vector2 const& _v);
		Vector2& operator*=(float _s);
		Vector2& operator/=(Vector2 const& _v);
		Vector2& operator/=(float _s);

		float Length() const;
		float LengthSquared() const;
		float Dot(Vector2 const& _v) const;
		Vector2 Cross(Vector2 const& _v) const;
		void Normalize();
		void Clamp(Vector2 const& _min, Vector2 const& _max);

		static float Distance(Vector2 const& _v1, Vector2 const& _v2);
		static float DistanceSquared(Vector2 const& _v1, Vector2 const& _v2);
		static Vector2 Min(Vector2 const& _v1, Vector2 const& _v2);
		static Vector2 Max(Vector2 const& _v1, Vector2 const& _v2);
		static Vector2 Transform(Vector2 const& _v, Matrix const& _m);
		static Vector2 TransformNormal(Vector2 const& _v, Matrix const& _m);

		static Vector2 const Zero;
		static Vector2 const One;
		static Vector2 const UnitX;
		static Vector2 const UnitY;

		float x;
		float y;
	};

	struct PHYSICS_DLL Vector3
	{
		Vector3() = default;
		Vector3(Vector3 const&) = default;
		Vector3(Vector3&&) noexcept = default;
		Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
		Vector3(DirectX::XMFLOAT3 const& _v) { memcpy_s(this, sizeof(Vector3), &_v, sizeof(Vector3)); }

		Vector3(DirectX::FXMVECTOR _v) { DirectX::XMStoreFloat3(reinterpret_cast<DirectX::XMFLOAT3*>(this), _v); }
		operator DirectX::XMVECTOR() const { return DirectX::XMLoadFloat3(reinterpret_cast<DirectX::XMFLOAT3 const*>(this)); }

		Vector3 operator+() const { return *this; }
		Vector3 operator-() const { return Vector3(-x, -y, -z); }

		Vector3 operator+(Vector3 const& _v) const;
		Vector3 operator-(Vector3 const& _v) const;
		Vector3 operator*(Vector3 const& _v) const;
		Vector3 operator*(float _s) const;
		Vector3 operator/(Vector3 const& _v) const;
		Vector3 operator/(float _s) const;

		bool operator==(Vector3 const& _v) const;
		bool operator!=(Vector3 const& _v) const;

		Vector3& operator=(Vector3 const&) = default;
		Vector3& operator=(Vector3&&) noexcept = default;
		Vector3& operator+=(Vector3 const& _v);
		Vector3& operator-=(Vector3 const& _v);
		Vector3& operator*=(Vector3 const& _v);
		Vector3& operator*=(float _s);
		Vector3& operator/=(Vector3 const& _v);
		Vector3& operator/=(float _s);

		float Length() const;
		float LengthSquared() const;
		float Dot(Vector3 const& _v) const;
		Vector3 Cross(Vector3 const& _v) const;
		void Normalize();
		void Clamp(Vector3 const& _min, Vector3 const& _max);

		static float Distance(Vector3 const& _v1, Vector3 const& _v2);
		static float DistanceSquared(Vector3 const& _v1, Vector3 const& _v2);
		static Vector3 Min(Vector3 const& _v1, Vector3 const& _v2);
		static Vector3 Max(Vector3 const& _v1, Vector3 const& _v2);
		static Vector3 Transform(Vector3 const& _v, Matrix const& _m);
		static Vector3 TransformNormal(Vector3 const& _v, Matrix const& _m);

		static Vector3 const Zero;
		static Vector3 const One;
		static Vector3 const UnitX;
		static Vector3 const UnitY;
		static Vector3 const UnitZ;

		float x;
		float y;
		float z;
	};

	struct PHYSICS_DLL Vector4
	{
		Vector4() = default;
		Vector4(Vector4 const&) = default;
		Vector4(Vector4&&) noexcept = default;
		Vector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
		Vector4(DirectX::XMFLOAT4 const& _v) { memcpy_s(this, sizeof(Vector4), &_v, sizeof(Vector4)); }

		Vector4(DirectX::FXMVECTOR _v) { DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4*>(this), _v); }
		operator DirectX::XMVECTOR() const { return DirectX::XMLoadFloat4(reinterpret_cast<DirectX::XMFLOAT4 const*>(this)); }

		Vector4 operator+() const { return *this; }
		Vector4 operator-() const { return Vector4(-x, -y, -z, -w); }

		Vector4 operator+(Vector4 const& _v) const;
		Vector4 operator-(Vector4 const& _v) const;
		Vector4 operator*(Vector4 const& _v) const;
		Vector4 operator*(float _s) const;
		Vector4 operator/(Vector4 const& _v) const;
		Vector4 operator/(float _s) const;

		bool operator==(Vector4 const& _v) const;
		bool operator!=(Vector4 const& _v) const;

		Vector4& operator=(Vector4 const&) = default;
		Vector4& operator=(Vector4&&) noexcept = default;
		Vector4& operator+=(Vector4 const& _v);
		Vector4& operator-=(Vector4 const& _v);
		Vector4& operator*=(Vector4 const& _v);
		Vector4& operator*=(float _s);
		Vector4& operator/=(Vector4 const& _v);
		Vector4& operator/=(float _s);

		float Length() const;
		float LengthSquared() const;
		float Dot(Vector4 const& _v) const;
		Vector4 Cross(Vector4 const& _v1, Vector4 const& _v2) const;
		void Normalize();
		void Clamp(Vector4 const& _min, Vector4 const& _max);

		static float Distance(Vector4 const& _v1, Vector4 const& _v2);
		static float DistanceSquared(Vector4 const& _v1, Vector4 const& _v2);
		static Vector4 Min(Vector4 const& _v1, Vector4 const& _v2);
		static Vector4 Max(Vector4 const& _v1, Vector4 const& _v2);
		static Vector4 Transform(Vector4 const& _v, Matrix const& _m);

		static Vector4 const Zero;
		static Vector4 const One;
		static Vector4 const UnitX;
		static Vector4 const UnitY;
		static Vector4 const UnitZ;
		static Vector4 const UnitW;

		float x;
		float y;
		float z;
		float w;
	};

	struct PHYSICS_DLL Quaternion
	{
		Quaternion() : x(0.f), y(0.f), z(0.f), w(1.f) {}
		Quaternion(Quaternion const&) = default;
		Quaternion(Quaternion&&) noexcept = default;
		Quaternion(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
		Quaternion(Vector3 const& _v, float _s) : x(_v.x), y(_v.y), z(_v.z), w(_s) {}
		Quaternion(DirectX::XMFLOAT4 const& _q) { memcpy_s(this, sizeof(Quaternion), &_q, sizeof(Quaternion)); }

		Quaternion(DirectX::FXMVECTOR _q) { DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4*>(this), _q); }
		operator DirectX::XMVECTOR() const { return DirectX::XMLoadFloat4(reinterpret_cast<DirectX::XMFLOAT4 const*>(this)); }

		Quaternion operator+() const { return *this; }
		Quaternion operator-() const { return Quaternion(-x, -y, -z, -w); }

		Quaternion operator+(Quaternion const& _q) const;
		Quaternion operator-(Quaternion const& _q) const;
		Quaternion operator*(Quaternion const& _q) const;
		Quaternion operator*(float _s) const;
		Quaternion operator/(Quaternion const& _q) const;
		Quaternion operator/(float _s) const;

		bool operator==(Quaternion const& _q) const;
		bool operator!=(Quaternion const& _q) const;

		Quaternion& operator=(Quaternion const&) = default;
		Quaternion& operator=(Quaternion&&) noexcept = default;
		Quaternion& operator+=(Quaternion const& _q);
		Quaternion& operator-=(Quaternion const& _q);
		Quaternion& operator*=(Quaternion const& _q);
		Quaternion& operator*=(float _s);
		Quaternion& operator/=(Quaternion const& _q);
		Quaternion& operator/=(float _s);

		static Quaternion CreateFromAxisAngle(Vector3 const& _axis, float _angle);
		static Quaternion CreateFromYawPitchRoll(float _yaw, float _pitch, float _roll);
		static Quaternion CreateFromRotationMatrix(Matrix const& _m);
		static Quaternion Slerp(Quaternion const& _q1, Quaternion const& _q2, float _t);

		static Quaternion const Identity;

		float x;
		float y;
		float z;
		float w;
	};

	struct PHYSICS_DLL Matrix
	{
		Matrix() = default;
		Matrix(Matrix const&) = default;
		Matrix(Matrix&&) noexcept = default;
		Matrix(
			float _m00, float _m01, float _m02, float _m03,
			float _m10, float _m11, float _m12, float _m13,
			float _m20, float _m21, float _m22, float _m23,
			float _m30, float _m31, float _m32, float _m33) :
			_11(_m00), _12(_m01), _13(_m02), _14(_m03),
			_21(_m10), _22(_m11), _23(_m12), _24(_m13),
			_31(_m20), _32(_m21), _33(_m22), _34(_m23),
			_41(_m30), _42(_m31), _43(_m32), _44(_m33) {}
		Matrix(Vector4 const& _r0, Vector4 const& _r1, Vector4 const& _r2, Vector4 const& _r3) :
			_11(_r0.x), _12(_r0.y), _13(_r0.z), _14(_r0.w),
			_21(_r1.x), _22(_r1.y), _23(_r1.z), _24(_r1.w),
			_31(_r2.x), _32(_r2.y), _33(_r2.z), _34(_r2.w),
			_41(_r3.x), _42(_r3.y), _43(_r3.z), _44(_r3.w) {}
		Matrix(DirectX::XMFLOAT4X4 const& _m) { memcpy_s(this, sizeof(Matrix), &_m, sizeof(Matrix)); }

		Matrix(DirectX::FXMMATRIX _m) { DirectX::XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(this), _m); }
		operator DirectX::XMMATRIX() const { return DirectX::XMLoadFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4 const*>(this)); }

		Matrix operator+() const { return *this; }
		Matrix operator-() const
		{
			return Matrix(
				-_11, -_12, -_13, -_14,
				-_21, -_22, -_23, -_24,
				-_31, -_32, -_33, -_34,
				-_41, -_42, -_43, -_44
			);
		}

		Matrix operator+(Matrix const& _m) const;
		Matrix operator-(Matrix const& _m) const;
		Matrix operator*(Matrix const& _m) const;
		Matrix operator*(float _s) const;
		Matrix operator/(Matrix const& _m) const;
		Matrix operator/(float _s) const;

		bool operator==(Matrix const& _m) const;
		bool operator!=(Matrix const& _m) const;

		Matrix& operator=(Matrix const&) = default;
		Matrix& operator=(Matrix&&) noexcept = default;
		Matrix& operator+=(Matrix const& _m);
		Matrix& operator-=(Matrix const& _m);
		Matrix& operator*=(Matrix const& _m);
		Matrix& operator*=(float _s);
		Matrix& operator/=(Matrix const& _m);
		Matrix& operator/=(float _s);

		Vector3 Right() const { return Vector3{ _11, _12, _13 }; }
		Vector3 Up() const { return Vector3{ _21, _22, _23 }; }
		Vector3 Forward() const { return Vector3{ _31, _32, _33 }; }
		Vector3 Translation() const { return Vector3{ _41, _42, _43 }; }

		Matrix Transpose() const;
		Matrix Inverse() const;

		static Matrix CreateScaling(Vector3 const& _v);
		static Matrix CreateRotationX(float _angle);
		static Matrix CreateRotationY(float _angle);
		static Matrix CreateRotationZ(float _angle);
		static Matrix CreateFromAxisAngle(Vector3 const& _axis, float _angle);
		static Matrix CreateFromYawPitchRoll(float _yaw, float _pitch, float _roll);
		static Matrix CreateFromQuaternion(Quaternion const& _q);
		static Matrix CreateTranslation(Vector3 const& _v);
		static Matrix CreateLookAt(Vector3 const& _eye, Vector3 const& _focus, Vector3 const& _up);
		static Matrix CreatePerspectiveFieldOfView(float _fov_angle, float _aspect_ratio, float _near, float _far);
		static Matrix CreatePerspective(float _width, float _height, float _near, float _far);
		static Matrix CreatePerspectiveOffCenter(float _left, float _right, float _bottom, float _top, float _near, float _far);
		static Matrix CreateOrthographic(float _width, float _height, float _near, float _far);
		static Matrix CreateOrthographicOffCenter(float _left, float _right, float _bottom, float _top, float _near, float _far);
		static Matrix Transform(Matrix const& _m, Quaternion const& _q);

		static Matrix const Identity;

		union
		{
			struct
			{
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			float m[4][4];
		};
	};
}
