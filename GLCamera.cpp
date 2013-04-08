#include "GLCamera.h"
#include <math.h>
#include <string.h>
#include <iostream>	//// REMOVE THIS IF NOT USING ANY OUTPUT

using namespace std;	//// REMOVE THIS IF NOT USING ANY OUTPUT

Camera::Camera()
{
	ModelMatrix = IDENTITY_MATRIX;
	ViewMatrix = IDENTITY_MATRIX;
	ProjectionMatrix = IDENTITY_MATRIX;
	xRot = 0;
	yRot = 0;
	zRot = 0;
	panX = 0;
	panY = 0;
	zoomLevel = 1;

	moveRatio = 4;
	spinScale = PI/(180.0*moveRatio);
	zoomScale = 1.1;
	tiltLimit = (90.0*PI)/180.0;


	window[0] = -1;
	window[1] = 1;
	window[2] = -1;
	window[3] = 1;
	window[4] = -1;
	window[5] = 1;

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	viewportX = viewport[2];
	viewportY = viewport[3];

	pixelToViewRatio = ((window[1]-window[0])/2.0)/viewportX;

}


Camera::~Camera()
{

}


void Camera::Pan(float dx, float dy)
{
	panX += (pixelToViewRatio*dx*cos(zRot) + pixelToViewRatio*dy*sin(zRot))/zoomLevel;
	panY += (pixelToViewRatio*dx*sin(zRot) - pixelToViewRatio*dy*cos(zRot))/zoomLevel;
	UpdateModel();
}


void Camera::Rotate(float rotateAmount)
{
	zRot -= rotateAmount*spinScale;
	UpdateModel();
}


void Camera::Tilt(float tiltAmount)
{

	if ((tiltAmount < 0 && xRot < tiltLimit) || (tiltAmount > 0 && xRot > 0))
	{
		xRot -= tiltAmount*spinScale;
	}

	UpdateModel();

}


void Camera::Zoom(float zoomAmount)
{
	if (zoomAmount > 0)
	{
		zoomLevel *= zoomScale;
	} else {
		zoomLevel /= zoomScale;
	}

	UpdateProjection();
}


void Camera::SetWindowSize(float l, float r, float b, float t, float n, float f)
{
	window[0] = l;
	window[1] = r;
	window[2] = b;
	window[3] = t;
	window[4] = n;
	window[5] = f;

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	viewportX = viewport[2];
	viewportY = viewport[3];

	pixelToViewRatio = (window[1]-window[0])/viewportX;

	UpdateProjection();
}


void Camera::UpdateModel()
{
	ModelMatrix = IDENTITY_MATRIX;
	TranslateMatrix(&ModelMatrix, panX, panY, 0);
	RotateMatrixAboutZ(&ModelMatrix, zRot);
	RotateMatrixAboutX(&ModelMatrix, xRot);
	Matrix VP = MultiplyMatrices(&ViewMatrix, &ProjectionMatrix);
	MVPMatrix = MultiplyMatrices(&ModelMatrix, &VP);

}


void Camera::UpdateProjection()
{
	ProjectionMatrix = IDENTITY_MATRIX;

	ProjectionMatrix.m[0] = 2.0/(window[1]-window[0]);
	ProjectionMatrix.m[3] = -(window[1]+window[0])/(window[1]-window[0]);
	ProjectionMatrix.m[5] = 2.0/(window[3]-window[2]);
	ProjectionMatrix.m[7] = -(window[3]+window[2])/(window[3]-window[2]);
	ProjectionMatrix.m[10] = -2.0/(window[5]-window[4]);
	ProjectionMatrix.m[11] = -(window[5]+window[4])/(window[5]-window[4]);

	ScaleMatrix(&ProjectionMatrix, zoomLevel, zoomLevel, zoomLevel);

	Matrix VP = MultiplyMatrices(&ViewMatrix, &ProjectionMatrix);
	MVPMatrix = MultiplyMatrices(&ModelMatrix, &VP);
}


Matrix Camera::MultiplyMatrices(const Matrix *m1, const Matrix *m2)
{
	Matrix out = IDENTITY_MATRIX;
	unsigned int row, column, row_offset;

	for (row=0, row_offset=row*4; row<4; ++row, row_offset=row*4)
	{
		for (column=0; column<4; ++column)
		{
			out.m[row_offset + column] =
				(m1->m[row_offset + 0] * m2->m[column + 0]) +
				(m1->m[row_offset + 1] * m2->m[column + 4]) +
				(m1->m[row_offset + 2] * m2->m[column + 8]) +
				(m1->m[row_offset + 3] * m2->m[column + 12]);
		}
	}

	return out;
}


void Camera::ScaleMatrix(Matrix *m, float x, float y, float z)
{
	Matrix scale = IDENTITY_MATRIX;

	scale.m[0] = x;
	scale.m[5] = y;
	scale.m[10] = z;

	memcpy(m->m, MultiplyMatrices(m, &scale).m, sizeof(m->m));
}


void Camera::TranslateMatrix(Matrix *m, float x, float y, float z)
{
	Matrix translation = IDENTITY_MATRIX;

	translation.m[12] = x;
	translation.m[13] = y;
	translation.m[14] = z;

	memcpy(m->m, MultiplyMatrices(m, &translation).m, sizeof(m->m));
}


void Camera::RotateMatrixAboutX(Matrix *m, float angle)
{
	Matrix rotation = IDENTITY_MATRIX;
	float sine = (float)sin(angle);
	float cosine = (float)cos(angle);

	rotation.m[5] = cosine;
	rotation.m[6] = -sine;
	rotation.m[9] = sine;
	rotation.m[10] = cosine;

	memcpy(m->m, MultiplyMatrices(m, &rotation).m, sizeof(m->m));
}


void Camera::RotateMatrixAboutZ(Matrix *m, float angle)
{
	Matrix rotation = IDENTITY_MATRIX;
	float sine = (float)sin(angle);
	float cosine = (float)cos(angle);

	rotation.m[0] = cosine;
	rotation.m[1] = -sine;
	rotation.m[4] = sine;
	rotation.m[5] = cosine;

	memcpy(m->m, MultiplyMatrices(m, &rotation).m, sizeof(m->m));
}
