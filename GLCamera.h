#ifndef GLCAMERA_H
#define GLCAMERA_H

#include "adcData.h"
#include "GLData.h"


class Camera {
	public:
		// Variable Definitions
		Matrix ModelMatrix;
		Matrix ViewMatrix;
		Matrix ProjectionMatrix;
		Matrix MVPMatrix;
		float xRot;
		float yRot;
		float zRot;
		float panX;
		float panY;
		float zoomLevel;

		int moveRatio;
		float spinScale;
		float zoomScale;
		float tiltLimit;

		float window[6];
		float viewportX;
		float viewportY;
		float pixelToViewRatio;


		// Function Definitions
		Camera();
		~Camera();
		void Pan(float dx, float dy);
		void Rotate(float rotateAmount);
		void Tilt(float tiltAmount);
		void Zoom(float zoomAmount);
		void SetWindowSize(float l, float r, float b, float t, float n, float f);

	private:

		// Update Matrices Operations
		void UpdateModel();
		void UpdateProjection();


		// Matrix Operations
		Matrix MultiplyMatrices(const Matrix* m1, const Matrix* m2);
		void ScaleMatrix(Matrix* m, float x, float y, float z);
		void TranslateMatrix(Matrix* m, float x, float y, float z);
		void RotateMatrixAboutX(Matrix* m, float angle);
		void RotateMatrixAboutZ(Matrix* m, float angle);


};

#endif // GLCAMERA_H
