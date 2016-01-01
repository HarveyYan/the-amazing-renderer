#pragma once

#include <iosfwd>
#include "Vector4d.h"
#include "Vertex.h"

const int DIM = 4;

/* Homogenous 4d matrix class. */
class Matrix4d
{
public:
	/* Default ctor. Sets the coordinate (4,4) to 1 since this is a homogenous matrix. */
	Matrix4d();
	/* Again, (4,4) is set to 1. */
	Matrix4d(
		double x11, double x12, double x13, 
		double x21, double x22, double x23, 
		double x31, double x32, double x33);
	Matrix4d(
		double x11, double x12, double x13, double x14,
		double x21, double x22, double x23, double x24,
		double x31, double x32, double x33, double x34,
		double x41, double x42, double x43, double x44);
	Matrix4d(const Matrix4d & rhs);
	~Matrix4d();

	/* Getters and setters. */
	double get(int row, int col) const;
	void set(int row, int col, double x);

	/* Operators. */
	Matrix4d & operator=(const Matrix4d & rhs);
	Matrix4d & operator*=(const Matrix4d & rhs);
	Matrix4d & operator+=(const Matrix4d & rhs);
	Matrix4d operator*(const Matrix4d & rhs) const;
	Matrix4d operator+(const Matrix4d & rhs) const;
	Vector4d operator*(const Vector4d & rhsVec) const;
	///* Note: this operator checks for equality with limited percision (since we are working with doubles). */
	bool operator==(const Matrix4d &rhs) const;

	friend std::ostream & operator<<(std::ostream & out, const Matrix4d & rhs);

private:
	double arr[DIM][DIM];
};

/********************/
/* Useful matrices. */
/********************/

/* Return the identity matrix. */
Matrix4d matId();

/* Returns a zero matrix. 
The index (4,4) is set to zero as well (it's usually set to 1 because of homogenous coordinates). */
Matrix4d trueZeroMat();

Matrix4d zeroMat();

extern const Matrix4d MAT4D_ORTH_PROJ;
extern const Matrix4d TRUE_ZERO_MAT;
extern const Matrix4d HOM_ZERO_MAT;
extern const Matrix4d ID_MAT;

/*******************/
/* Transformations */
/*******************/

Matrix4d translate(const double dx, const double dy, const double dz);
Matrix4d scale(const double sx, const double sy, const double sz);
Matrix4d scale(const double s);
Matrix4d rotatex(const double wx);
Matrix4d rotatey(const double wy);
Matrix4d rotatez(const double wz);
Matrix4d rotate(const double wx, const double wy, const double wz);
Matrix4d translateInv(const double dx, const double dy, const double dz);
Matrix4d scaleInv(const double sx, const double sy, const double sz);
Matrix4d rotateInv(const double wx, const double wy, const double wz);

/* Printing functions. */

void log_debug_matrix(const Matrix4d & mat);

