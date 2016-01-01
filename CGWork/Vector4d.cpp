#include <cmath>
#include "Vector4d.h"
#include "myHelpers.h"

Vector4d::~Vector4d()
{
}

bool Vector4d::operator==(Vector4d &rhs) {
	if (this == &rhs) {
		return true;
	}
	return almost_eq(getX(), rhs.getX()) && almost_eq(getY(), rhs.getY()) && almost_eq(getZ(), rhs.getZ());
}

double Vector4d::operator*(const Vector4d & rhs) const {
	return (this->x*rhs.x + this->y*rhs.y + this->z*rhs.z) / (this->h*rhs.h);
}

Vector4d Vector4d::operator/(double rhs) const {
	if (almost_eq(rhs, 0)) {
		log_warning("rhs == %f is close or equal to zero\n", rhs);
	}
	return Vector4d(this->x / rhs, this->y / rhs, this->z / rhs, this->h);
}

Vector4d operator*(double lhs, const Vector4d & rhs) {
	return Vector4d(lhs*rhs.x, lhs*rhs.y, lhs*rhs.z, rhs.h);
}

Vector4d Vector4d::operator-(const Vector4d & rhs) const {
	return Vector4d(this->getX() - rhs.getX(), this->getY() - rhs.getY(), this->getZ() - rhs.getZ());
}

Vector4d Vector4d::operator-() const {
	return Vector4d(-this->x, -this->y, -this->z, this->h);
}

double dot_product(const Vector4d & v1, const Vector4d & v2) {
	return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z) / (v1.h*v2.h);
}

Vector4d cross_product(const Vector4d & v1, const Vector4d & v2) {
	return Vector4d(
		v1.y*v2.z - v1.z*v2.y,
		v1.z*v2.x - v1.x*v2.z,
		v1.x*v2.y - v1.y*v2.x,
		v1.h*v2.h
		);
}

double norm2(const Vector4d & v) {
	return std::sqrt((v.x*v.x + v.y*v.y + v.z*v.z) / (v.h*v.h));
}

double distance(const Vector4d & v1, const Vector4d & v2) {
	double dx = v2.x*v1.h - v1.x*v2.h;
	double dy = v2.y*v1.h - v1.y*v2.h;
	double dz = v2.z*v1.h - v1.z*v2.h;
	double h12 = v1.h * v2.h;
	return std::sqrt((dx*dx+dy*dy+dz*dz)/(h12*h12));
}

Vector4d normalize(const Vector4d & v) {
	double l = (v.x*v.x + v.y*v.y + v.z*v.z) / v.h;
	return Vector4d(v.x / l, v.y / l, v.z / l, v.h);
}

void Vector4d::homegenize() {
	x = x / h;
	y = y / h;
	z = z / h;
	h = 1;
}

Vector4d Vector4d::operator+(const Vector4d & rhs) const {
	return Vector4d(getX() + rhs.getX(), getY() + rhs.getY(), getZ() + rhs.getZ(), 1);
}

void draw(CDC * pDC, const Vector4d & v1, const Vector4d & v2, COLORREF c) {
	midpoint_line_draw(pDC, (int)v1.getX(), (int)v1.getY(), (int)v2.getX(), (int)v2.getY(), c);
}

bool pointIsInRect(
	const Vector4d &P,
	const Vector4d &A,
	const Vector4d &B,
	const Vector4d &C,
	const Vector4d &D) 
{
	Vector4d AB = B - A;
	Vector4d AP = P - A;
	Vector4d AD = D - A;
	return 0 < dot_product(AB, AP) && dot_product(AB, AP) < dot_product(AB, AB) &&
		0 < dot_product(AP, AD) && dot_product(AP, AD) < dot_product(AD, AD);
}

// Find the point of intersection between the ray passing through P0 and P1 and the plane defined by
// the normal n and the number D (a point p is on the plane iff <n,p>=D).
bool rayPlaneIntersect(
	const Vector4d &P0, 
	const Vector4d &P1, 
	const Vector4d &n, 
	double D,
	Vector4d &intersectionPoint) 
{
	// P0 and P1 are two points on the ray, n is the normal to the plane, and D is a real number s.t
	// <n,p>=D for any p on the plane.
	//
	// Expressing the ray is parametric form, P(t)=P0+t(P1-P0)=P0+td, we wish to solve
	//     <(P0+td), n> = D
	// Giving us
	//     t = (D-<P0,n>)/<d,n>
	Vector4d d = P1 - P0;
	if (dot_product(d, n) == 0) { // ray is parallel to plane
		return false;
	}
	double t = (D - dot_product(P0, n)) / dot_product(d, n);
	intersectionPoint = P0 + t*d;
	return true;
}

bool pointInTriangle(const Vector4d &P, const Vector4d &A, const Vector4d &B, const Vector4d &C) {
	// Point is inside triangle if all cross products point in the same direction.
	Vector4d v1 = cross_product(B - A, P - A);
	Vector4d v2 = cross_product(C - B, P - B);
	Vector4d v3 = cross_product(A - C, P - C);
	if (dot_product(v1, v2) > 0 && dot_product(v1, v3) > 0 && dot_product(v2, v3) > 0) {
		return true;
	}
	return false;
}

// input: three points on a plane
// output: normal to the plane and a number d such that <p,n>=D for points on the plane.
void calculatePlane(const Vector4d &A, const Vector4d &B, const Vector4d &C, 
	Vector4d &norm, double &D) {
	norm = normalize(cross_product(B-A, C-B));
	D = dot_product(norm, A);
}

bool rayTriangleIntersection(
	const Vector4d &P0, 
	const Vector4d &P1, 
	const Vector4d &A, 
	const Vector4d &B,
	const Vector4d &C,
	Vector4d *intersectionPoint)
{
	double D;
	Vector4d norm, planeInterPt;
	calculatePlane(A, B, C, norm, D);
	if (!rayPlaneIntersect(P0, P1, norm, D, planeInterPt)) {
		return false;
	}
	if (intersectionPoint)
		*intersectionPoint = planeInterPt;
	return pointInTriangle(planeInterPt, A, B, C);
}

bool rayRectIntersection(
	const Vector4d &P0,
	const Vector4d &P1,
	const Vector4d &R1,
	const Vector4d &R2,
	const Vector4d &R3,
	const Vector4d &R4,
	Vector4d *intersectionPoint)
{
	double D;
	Vector4d norm, planeInterPt;
	calculatePlane(R1, R2, R3, norm, D);
	if (!rayPlaneIntersect(P0, P1, norm, D, planeInterPt)) {
		return false;
	}
	if (intersectionPoint) {
		*intersectionPoint = planeInterPt;
	}
	return pointIsInRect(planeInterPt, R1, R2, R3, R4);
}