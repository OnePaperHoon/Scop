#pragma once
#include <cmath>
#include <cstring>

constexpr float DEG2RAD = 3.1415926535f / 180.0f;

struct Vec3 {
	float x, y, z;

	bool operator==(const Vec3& other) const {
		return x == other.x && y == other.y && z == other.z;
	}
};

struct Vec2 {
	float x, y;

	bool operator==(const Vec2& other) const {
		return x == other.x && y == other.y;
	}
};

inline void multiplyMatrix(const float* a, const float* b, float* result)
{
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
		{
			result[row * 4 + col] =
				a[row * 4 + 0] * b[0 * 4 + col] +
				a[row * 4 + 1] * b[1 * 4 + col] +
				a[row * 4 + 2] * b[2 * 4 + col] +
				a[row * 4 + 3] * b[3 * 4 + col];
		}
	}
}

inline void rotateX(float angleDeg, float* matrix)
{
	float angleRad = angleDeg * DEG2RAD;
	float cosA = cosf(angleDeg);
	float sinA = sinf(angleDeg);

	float rot[16] = {
		1, 0, 0, 0,
		0, cosA, -sinA, 0,
		0, sinA, cosA, 0,
		0, 0, 0, 1
	};

	float result[16];
	multiplyMatrix(matrix, rot, result);
	memcpy(matrix, result, sizeof(result));
}

inline void rotateY(float angleDeg, float* matrix)
{
	float angleRad = angleDeg * DEG2RAD;
	float cosA = cosf(angleDeg);
	float sinA = sinf(angleDeg);

	float rot[16] = {
		cosA, 0, sinA, 0,
		0, 1, 0, 0,
		-sinA, 0, cosA, 0,
		0, 0, 0, 1
	};

	float result[16];
	multiplyMatrix(matrix, rot, result);
	memcpy(matrix, result, sizeof(result));
}

inline void createTranslationMatrix(float tx, float ty, float tz, float* matrix)
{
	for (int i = 0; i < 16; ++i)
		matrix[i] = 0.0f;

	matrix[0] = 1.0f;
	matrix[5] = 1.0f;
	matrix[10] = 1.0f;
	matrix[15] = 1.0f;

	matrix[12] = tx;
	matrix[13] = ty;
	matrix[14] = tz;
}

inline void setIdentityMatrix(float* outMatrix)
{
	for (int i = 0; i < 16; ++i)
		outMatrix[i] = 0.0f;

	outMatrix[0] = 1.0f;
	outMatrix[5] = 1.0f;
	outMatrix[10] = 1.0f;
	outMatrix[15] = 1.0f;
}

inline Vec3 subtract(const Vec3& a, const Vec3& b)
{
	return { a.x - b.x, a.y - b.y, a.z - b.z };
}

inline Vec3 normalize(const Vec3& v)
{
	float len = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	return { v.x / len, v.y / len, v.z / len };
}

inline Vec3 cross(const Vec3& a, const Vec3& b)
{
	return {
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	};
}

inline float dot(const Vec3& a, const Vec3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline void createLookAtMatrix(const Vec3& eye, const Vec3& center, const Vec3& up, float* matrix)
{
	Vec3 f = normalize(subtract(center, eye));
	Vec3 s = normalize(cross(f, up));
	Vec3 u = cross(s, f);

	matrix[0] = s.x; matrix[1] = u.x; matrix[2] = -f.x; matrix[3] = 0.0f;
	matrix[4] = s.y; matrix[5] = u.y; matrix[6] = -f.y; matrix[7] = 0.0f;
	matrix[8] = s.z; matrix[9] = u.z; matrix[10] = -f.z; matrix[11] = 0.0f;
	matrix[12] = -dot(s, eye); matrix[13] = -dot(u, eye); matrix[14] = dot(f, eye); matrix[15] = 1.0f;
}

inline void createPerspectiveMatrix(float fov, float aspect, float neer, float far, float* matrix)
{
	float rad = fov * DEG2RAD;
	float tanHalfFov = tan(rad / 2.0f);

	for (int i = 0; i < 16; ++i)
		matrix[i] = 0.0f;

	matrix[0] = 1.0f / (aspect * tanHalfFov);
	matrix[5] = 1.0f / tanHalfFov;
	matrix[10] = -(far + neer) / (far - neer);
	matrix[11] = -1.0f;
	matrix[14] = -(2.0f * far * neer) / (far - neer);
}
