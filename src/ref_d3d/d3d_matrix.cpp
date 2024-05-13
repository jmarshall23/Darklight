// d3d_matrix.cpp
//

#include "d3d_local.h"

Matrix4x4 MultiplyMatrix(const Matrix4x4& a, const Matrix4x4& b) {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = 0.0f;
			for (int k = 0; k < 4; ++k) {
				result.m[i][j] += a.m[i][k] * b.m[k][j];
			}
		}
	}
	return result;
}

Matrix4x4 glOrtho(float left, float right, float bottom, float top, float nearVal, float farVal) {
	Matrix4x4 result = {};

	// Set diagonal elements
	result.m[0][0] = 2.0f / (right - left);
	result.m[1][1] = 2.0f / (top - bottom);
	result.m[2][2] = 1.0f / (farVal - nearVal);
	result.m[3][3] = 1.0f;

	// Set last column for translation
	result.m[3][0] = -(right + left) / (right - left);
	result.m[3][1] = -(top + bottom) / (top - bottom);
	result.m[3][2] = -nearVal / (farVal - nearVal);

	return result;
}

void glTranslatef(Matrix4x4& matrix, float x, float y, float z) {
	Matrix4x4 translation;
	translation.LoadIdentity();  // Start with an identity matrix
	translation.m[3][0] = x;
	translation.m[3][1] = y;
	translation.m[3][2] = z;

	// Multiply the current matrix by the translation matrix
	matrix = MultiplyMatrix(matrix, translation);
}

void glRotatef(Matrix4x4& matrix, float angle, float x, float y, float z) {
	Matrix4x4 rotation;
	rotation.LoadIdentity();  // Start with an identity matrix

	// Calculate radians from degrees
	float radians = angle * 3.14159265358979323846f / 180.0f;
	float c = cosf(radians);
	float s = sinf(radians);

	if (x == 1.0f && y == 0.0f && z == 0.0f) {
		// Rotation around the x-axis
		rotation.m[1][1] = c;
		rotation.m[1][2] = s;
		rotation.m[2][1] = -s;
		rotation.m[2][2] = c;
	}
	else if (x == 0.0f && y == 1.0f && z == 0.0f) {
		// Rotation around the y-axis
		rotation.m[0][0] = c;
		rotation.m[0][2] = -s;
		rotation.m[2][0] = s;
		rotation.m[2][2] = c;
	}
	else if (x == 0.0f && y == 0.0f && z == 1.0f) {
		// Rotation around the z-axis
		rotation.m[0][0] = c;
		rotation.m[0][1] = -s;
		rotation.m[1][0] = s;
		rotation.m[1][1] = c;
	}

	// Multiply the current matrix by the rotation matrix
	matrix = MultiplyMatrix(matrix, rotation);
}

void glLoadIdentity(Matrix4x4& matrix) {
	matrix.LoadIdentity();
}

void R_SetGL2D(void) {	
	tr.mb.projectionMatrix = glOrtho(0, tr.s_window_width, tr.s_window_height, 0, -99999, 99999);
	glLoadIdentity(tr.mb.modelViewMatrix);
}