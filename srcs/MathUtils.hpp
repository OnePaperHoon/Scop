#pragma once
#include <cmath>
#include <cstring>

constexpr float DEG2RAD = 3.1415926535f / 180.0f;


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
