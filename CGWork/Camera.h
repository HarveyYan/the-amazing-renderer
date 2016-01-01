#pragma once

#include "Matrix4d.h"

class Vector4d;

// Orthographic camera class.
class OrthoCamera
{
public:
	OrthoCamera();
	~OrthoCamera();

	// Position the camera.
	void set(double l, double r, double t, double b, double n, double f);
	Matrix4d get() const;
	double getWidth() const { return width; }
	double getHeight() const { return height; }
	double getDepth() const { return depth; }

	Matrix4d cameraMat, normCubeMat;

private:
	Matrix4d mat, matInv;
	double width;
	double height;
	double depth;
};

// Perspective camera class.
class PerspCamera
{
public:
	PerspCamera() {};
	~PerspCamera() {};

	// Position the camera.
	void PerspCamera::set(
		const Vector4d & P, 
		const Vector4d & look, 
		const Vector4d & up, 
		double fovy, 
		double aspect, 
		double n, 
		double f);
	void set(double l, double r, double t, double b, double n, double f);
	void set(double fovx, double aspectRatio, double n, double f);
	Matrix4d get() const { return mat; }
	double getWidth() const { return width; }
	double getHeight() const { return height; }
	double getDepth() const { return depth; }

	Matrix4d cameraMat, normCubeMat;

private:
	Matrix4d mat, matInv;
	double width;
	double height;
	double depth;
};

