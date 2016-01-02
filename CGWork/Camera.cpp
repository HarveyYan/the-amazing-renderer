#include "Camera.h"
#include "myHelpers.h"
#include "Vector4d.h"
#define _USE_MATH_DEFINES
#include <math.h>


/***********************/
/* Orthonormal camera. */
/***********************/

OrthoCamera::OrthoCamera()
{
}

OrthoCamera::~OrthoCamera()
{
}

void OrthoCamera::set(double l, double r, double t, double b, double n, double f) {
	width = abs(r - l);
	height = abs(t - b);
	depth = abs(n - f);
	//mat = Matrix4d(
	//	2 / width, 0, 0, -(r + l) / width,
	//	0, 2 / height, 0, -(t + b) / height,
	//	0, 0, 2 / depth, (f + n) / depth, // TODO + or -?
	//	0, 0, 0, 1
	//	); // TODO
	cameraMat = translate(-(l + r) / 2, -(b + t) / 2, (n + f) / 2);
	//normCubeMat = scale(2 / (l - r), 2 / (t - b), 2 / (n - f)); // TODO remove minus from first?
	normCubeMat = scale(-2 / (l - r), 2 / (t - b), -2 / (n - f)); // TODO
	mat = normCubeMat * cameraMat;
	matInv =
		translateInv(- (l + r) / 2, -(b + t) / 2, (n + f) / 2) *
		scaleInv(2 / (l - r), 2 / (t - b), 2 / (n - f));
		//translate((r + l) / width, (t + b) / height, -(f + n) / depth) *
		//scale(width / 2, height / 2, depth / 2); // TODO
}

Matrix4d OrthoCamera::get() const {
	return mat;
}

/***********************/
/* Perspective camera. */
/***********************/

// Place the camera in the world pointing at a given direction.
void PerspCamera::set(
	const Vector4d & P,
	const Vector4d & look,
	const Vector4d & up,
	double fovy,
	double aspect,
	double n,
	double f)
{
	// Create camera coordinate system uvw.
	// u points to the right
	// v points up
	// w points behind the camera
	Vector4d w = -look / norm2(look);
	// project up onto the plane perpendicular to w
	Vector4d v = up - dot_product(up, w)*w;
	v = normalize(v);
	//Vector4d u = cross_product(w, v);
	Vector4d u = normalize(cross_product(v, w));

	// Calculate width and height, taking into account the aspect ratio.
	double theta_x, theta_y;
	depth = abs(f - n);
	if (aspect > 1) { // Stretch width
		theta_y = fovy * M_PI / 180;
		height = 2 * n*tan(theta_y / 2);
		width = height * aspect;
		theta_x = 2 * atan(width / 2);
	}
	else { // Squash height.
		theta_x = fovy * M_PI / 180;
		width = 2 * n*tan(theta_x / 2);
		height = width / aspect;
		theta_y = 2 * atan(height / 2);
	}

	cameraMat =
		scale(1 / (f*tan(theta_x / 2)), 1 / (f*tan(theta_y / 2)), 1 / f) *  // Normalize the frustum.
		Matrix4d(
		u.getX(), u.getY(), u.getZ(), 0,
		v.getX(), v.getY(), v.getZ(), 0,
		w.getX(), w.getY(), w.getZ(), 0,
		0, 0, 0, 1) * // Align object(?) coordinates with camera coordinates.
		translate(-P.getX(), -P.getY(), -P.getZ()); // Move camera center to origin.
	normCubeMat = Matrix4d(
		f - n, 0, 0, 0,
		0, f - n, 0, 0,
		0, 0, f, n,
		0, 0, -(f - n), 0); // Transform normalized frustum to a cube of width/height/depth of 2.
	mat = normCubeMat * cameraMat;

	Matrix4d MperInv =
		translate(P.getX(), P.getY(), P.getZ()) *
		Matrix4d(
		u.getX(), v.getX(), w.getX(), 0,
		u.getY(), v.getY(), w.getY(), 0,
		u.getZ(), v.getZ(), w.getZ(), 0,
		0, 0, 0, 1) *
		scale((f*tan(theta_x / 2)) / 1, (f*tan(theta_y / 2)) / 1, f / 1);
	Matrix4d MppInv(
		1 / (f - n), 0, 0, 0,
		0, 1 / (f - n), 0, 0,
		0, 0, 0, 1 / (n - f),
		0, 0, 1 / n, f / (f*n - n*n)
		);
	matInv = MperInv * MppInv;
}

void PerspCamera::set(double l, double r, double t, double b, double n, double f) {
	width = abs(r - l);
	height = abs(t - b);
	depth = abs(n - f);
	mat =
		Matrix4d(
		2 / width, 0, 0, 0,
		0, 2 / height, 0, 0,
		0, 0, (n + f) / (f - n), (2 * n * f) / (n - f),
		0, 0, 1, 0
		) * translate(0, 0, n);
}

void PerspCamera::set(double fovx, double aspectRatio, double n, double f) {
	static int i = 0;
	double theta_x = fovx * M_PI / 180;
	double t = tan(theta_x / 2)*n;
	double b = -t;
	double r = t * aspectRatio;
	double l = -t * aspectRatio;

	log_debug("t, b = %f, %f\n", t, b);
	log_debug("r, l = %f, %f\n", r, l);
	log_debug("f, n = %f,  %f\n", f, n);

	mat = Matrix4d(
		2 * n / (r - l), 0, (r + l) / (r - l), 0,
		0, 2 * n / (t - b), (t + b) / (t - b), 0,
		0, 0, -(f + n) / (f - n), -2 * f * n / (f - n),
		0, 0, -1, 0);

	/*matInv = Matrix4d(
		-0.5*(l - r) / n, 0, 0, 0.5*(l + r) / n,
		0, -0.5*(b - t) / n, 0, 0.5*(b + t) / n,
		0, 0, 0, -1,
		0, 0, 0.5*(n - f) / (f*n), 0.5*(f + n) / (f*n));*/
}