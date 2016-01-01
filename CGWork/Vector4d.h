#pragma once
#include "StdAfx.h"

/* Homegenous vector structure. The four dimensional vector (x,y,z,h) represents the three dimensional vector (x/h,y/h,z/h).*/
class Vector4d
{
public:
	/* Constructors and destructors. */
	Vector4d() : x(0), y(0), z(0), h(1) {};
	Vector4d(double _x, double _y, double _z) : x(_x), y(_y), z(_z), h(1) {};
	Vector4d(double _x, double _y, double _z, double _w) : x(_x), y(_y), z(_z), h(_w) {};
	~Vector4d();

	/* Getters and setters. */
	// Return the unhomegenous value as is (no division by h).
	double getXUnhom() const { return x; }
	double getYUnhom() const { return y; }
	double getZUnhom() const { return z; }
	// Return the homengenous value (meaning divided by h).
	double getX() const { return x / h; }
	double getY() const { return y / h; }
	double getZ() const { return z / h; }
	double getH() const { return h; }
	// Set does not take into account the value of h.
	void setX(double _x) { x = _x; }
	void setY(double _y) { y = _y; }
	void setZ(double _z) { z = _z; }
	void setH(double _h) { h = _h; }

	/* Operators and vector functions. */
	friend double dot_product(const Vector4d & v1, const Vector4d & v2);
	friend Vector4d cross_product(const Vector4d & v1, const Vector4d & v2);
	friend double norm2(const Vector4d & v);
	friend double distance(const Vector4d & v1, const Vector4d & v2);
	friend Vector4d normalize(const Vector4d & v);
	friend Vector4d operator*(double lhs, const Vector4d & rhs);
	/* Note: this operator checks for equality with limited percision (since we are working with double). */
	bool operator==(Vector4d &rhs);
	double operator*(const Vector4d & rhs) const;
	Vector4d operator/(double rhs) const;
	Vector4d operator-() const;
	Vector4d operator-(const Vector4d & rhs) const;
	Vector4d operator+(const Vector4d & rhs) const;

	// Divide x y and z by h and set h to be 1.
	void homegenize();

private:
	double x, y, z, h;
};

double dot_product(const Vector4d & v1, const Vector4d & v2);
Vector4d cross_product(const Vector4d & v1, const Vector4d & v2);
double norm2(const Vector4d & v);
double distance(const Vector4d & v1, const Vector4d & v2);
Vector4d normalize(const Vector4d & v);
Vector4d operator*(double lhs, const Vector4d & rhs);

void draw(CDC * pDC, const Vector4d & v1, const Vector4d & v2, COLORREF c);

bool pointIsInRect(
	const Vector4d &P, 
	const Vector4d &A,
	const Vector4d &B, 
	const Vector4d &C, 
	const Vector4d &D);
bool rayPlaneIntersect(const Vector4d &P0, const Vector4d &P1, const Vector4d &n, double D);	
bool pointInTriangle(const Vector4d &P, const Vector4d &A, const Vector4d &B, const Vector4d &C);
void calculatePlane(const Vector4d &A, const Vector4d &B, const Vector4d &C, Vector4d &norm, double &D);
bool rayTriangleIntersection(
	const Vector4d &P0,
	const Vector4d &P1,
	const Vector4d &A,
	const Vector4d &B,
	const Vector4d &C,
	Vector4d *intersectionPoint = NULL);
bool rayRectIntersection(
	const Vector4d &P0,
	const Vector4d &P1,
	const Vector4d &R1,
	const Vector4d &R2,
	const Vector4d &R3,
	const Vector4d &R4,
	Vector4d *intersectionPoint = NULL);