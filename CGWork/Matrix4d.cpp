#include <iostream>
#include "Matrix4d.h"
#include "myHelpers.h"
#include "Vertex.h"

/*****************/
/* Constructors. */
/*****************/

Matrix4d::Matrix4d()
{
	for (int row = 0; row < DIM; ++row) {
		for (int col = 0; col < DIM; ++col) {
			arr[row][col] = 0;
		}
	}
	arr[DIM-1][DIM-1] = 1;
}

Matrix4d::Matrix4d(
	double x11, double x12, double x13, 
	double x21, double x22, double x23, 
	double x31, double x32, double x33) :
		Matrix4d(
			x11,x12,x13,0,
			x21,x22,x23,0,
			x31,x32,x33,0,
			0,0,0,1
		)
{
}

Matrix4d::Matrix4d(
	double x11, double x12, double x13, double x14,
	double x21, double x22, double x23, double x24,
	double x31, double x32, double x33, double x34,
	double x41, double x42, double x43, double x44) 
{
	arr[0][0] = x11; arr[0][1] = x12; arr[0][2] = x13; arr[0][3] = x14;
	arr[1][0] = x21; arr[1][1] = x22; arr[1][2] = x23; arr[1][3] = x24;
	arr[2][0] = x31; arr[2][1] = x32; arr[2][2] = x33; arr[2][3] = x34;
	arr[3][0] = x41; arr[3][1] = x42; arr[3][2] = x43; arr[3][3] = x44;
}

Matrix4d::Matrix4d(const Matrix4d & rhs) {
	arr[0][0] = rhs.arr[0][0]; arr[0][1] = rhs.arr[0][1]; arr[0][2] = rhs.arr[0][2]; arr[0][3] = rhs.arr[0][3];
	arr[1][0] = rhs.arr[1][0]; arr[1][1] = rhs.arr[1][1]; arr[1][2] = rhs.arr[1][2]; arr[1][3] = rhs.arr[1][3];
	arr[2][0] = rhs.arr[2][0]; arr[2][1] = rhs.arr[2][1]; arr[2][2] = rhs.arr[2][2]; arr[2][3] = rhs.arr[2][3];
	arr[3][0] = rhs.arr[3][0]; arr[3][1] = rhs.arr[3][1]; arr[3][2] = rhs.arr[3][2]; arr[3][3] = rhs.arr[3][3];
}


Matrix4d::~Matrix4d()
{
}

/************************/
/* Getters and setters. */
/************************/

double Matrix4d::get(int row, int col) const {
	if (row < 0 || row >= DIM || col < 0 || col >= DIM) {
		std::cerr << "Invalid index: Matrix4d::get(" << row << ", " << col << ")" << std::endl;
		exit(1);
	}
	return arr[row][col];
}

void Matrix4d::set(int row, int col, double x) {
	if (row < 0 || row >= DIM || col < 0 || col >= DIM) {
		std::cerr << "Invalid index: Matrix4d::set(" << row << ", " << col << ")" << std::endl;
		exit(1);
	}
	arr[row][col] = x;
}

/**************/
/* Operators. */
/**************/

Matrix4d & Matrix4d::operator=(const Matrix4d & rhs) {
	if (this == &rhs) {
		return *this;
	}
	for (int row = 0; row < DIM; ++row) {
		for (int col = 0; col < DIM; ++col) {
			this->arr[row][col] = rhs.arr[row][col];
		}
	}
	return *this;
}

Matrix4d & Matrix4d::operator*=(const Matrix4d & rhs) {
	Matrix4d res = TRUE_ZERO_MAT;
	for (int row = 0; row < DIM; ++row) {
		for (int col = 0; col < DIM; ++col) {
			for (int k = 0; k < DIM; ++k) {
				res.arr[row][col] += this->arr[row][k] * rhs.arr[k][col];
			}
		}
	}
	*this = res;
	return *this;
}

Matrix4d & Matrix4d::operator+=(const Matrix4d & rhs) {
	for (int row = 0; row < DIM; ++row) {
		for (int col = 0; col < DIM; ++col) {
			this->arr[row][col] += rhs.arr[row][col];
		}
	}
	return *this;
}

Matrix4d Matrix4d::operator*(const Matrix4d & rhs) const {
	Matrix4d res = trueZeroMat();
	for (int row = 0; row < DIM; ++row) {
		for (int col = 0; col < DIM; ++col) {
			for (int k = 0; k < DIM; ++k) {
				res.arr[row][col] += this->arr[row][k] * rhs.arr[k][col];
			}
		}
	}
	return res;
}

Matrix4d Matrix4d::operator+(const Matrix4d & rhs) const {
	Matrix4d res(*this);
	//Matrix4d res = TRUE_ZERO_MAT;
	//res += *this;
	res += rhs;
	return res;
}

Vector4d Matrix4d::operator*(const Vector4d & rhsVec) const {
	double res[DIM] = { 0 };
	for (int row = 0; row < DIM; ++row) {
		res[row] = this->get(row, 0) * rhsVec.getXUnhom() +
			this->get(row, 1) * rhsVec.getYUnhom() +
			this->get(row, 2) * rhsVec.getZUnhom() +
			this->get(row, 3) * rhsVec.getH();
	}
	return Vector4d(res[0], res[1], res[2], res[3]);
}

bool Matrix4d::operator==(const Matrix4d &rhs) const {
	if (this == &rhs) {
		return true;
	}
	for (int row = 0; row < DIM; ++row) {
		for (int col = 0; col < DIM; ++col) {
			if (!almost_eq(this->arr[row][col], rhs.arr[row][col])) {
				return false;
			}
		}
	}
	return true;
}

std::ostream & operator<<(std::ostream & out, const Matrix4d & rhs) {
	for (int row = 0; row < DIM; ++row) {
		for (int col = 0; col < DIM; ++col) {
			out << rhs.arr[row][col] << " ";
		}
		out << std::endl;
	}
	return out;
}

/*******************/
/* Useful matrices */
/*******************/

Matrix4d matId() {
	return Matrix4d(
		1, 0, 0, 
		0, 1, 0, 
		0, 0, 1);
}

Matrix4d trueZeroMat() {
	return Matrix4d(
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0);
}

Matrix4d zeroMat() {
	return Matrix4d(
		0, 0, 0, 0, 
		0, 0, 0, 0, 
		0, 0, 0, 0, 
		0, 0, 0, 1);
}

const Matrix4d MAT4D_ORTH_PROJ(
	1, 0, 0, 
	0, 1, 0, 
	0, 0, 0);

extern const Matrix4d TRUE_ZERO_MAT(
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0);

extern const Matrix4d HOM_ZERO_MAT(
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 1);

extern const Matrix4d ID_MAT(
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1);

Matrix4d translate(const double dx, const double dy, const double dz) {
	return Matrix4d(
		1, 0, 0, dx,
		0, 1, 0, dy,
		0, 0, 1, dz,
		0, 0, 0, 1);
}

Matrix4d scale(const double sx, const double sy, const double sz) {
	return Matrix4d(
		sx, 0, 0,
		0, sy, 0,
		0, 0, sz);
}

Matrix4d scale(const double s) {
	return Matrix4d(
		s, 0, 0,
		0, s, 0,
		0, 0, s);
}

Matrix4d rotatex(const double wx) {
	return Matrix4d(1, 0, 0, 0, cos(wx), -sin(wx), 0, sin(wx), cos(wx));
}

Matrix4d rotatey(const double wy) {
	return Matrix4d(cos(wy), 0, sin(wy), 0, 1, 0, -sin(wy), 0, cos(wy));
}

Matrix4d rotatez(const double wz) {
	return Matrix4d(cos(wz), -sin(wz), 0, sin(wz), cos(wz), 0, 0, 0, 1);
}

// yzx rotation
Matrix4d rotate(const double wx, const double wy, const double wz) {
	return Matrix4d(1, 0, 0, 0, cos(wx), -sin(wx), 0, sin(wx), cos(wx)) *
		Matrix4d(cos(wy), 0, sin(wy), 0, 1, 0, -sin(wy), 0, cos(wy)) *
		Matrix4d(cos(wz), -sin(wz), 0, sin(wz), cos(wz), 0, 0, 0, 1);
}

Matrix4d translateInv(const double dx, const double dy, const double dz) {
	return Matrix4d(
		1, 0, 0, -dx,
		0, 1, 0, -dy,
		0, 0, 1, -dz,
		0, 0, 0, 1);
}
Matrix4d scaleInv(const double sx, const double sy, const double sz) {
	if (sx == 0 || sy == 0 || sz == 0) {
		log_error("Division by zero!\n");
		exit(1);
	}
	return Matrix4d(
		1 / sx, 0, 0,
		0, 1 / sy, 0,
		0, 0, 1 / sz);
}

// inverse zyx rotation.
Matrix4d rotateInv(const double wx, const double wy, const double wz) {
	return
		Matrix4d(
		1, 0, 0,
		0, cos(wx), sin(wx),
		0, -sin(wx), cos(wx)
		) *
		Matrix4d(
		cos(wy), 0, -sin(wy),
		0, 0, 0,
		sin(wy), 0, cos(wy)
		) *
		Matrix4d(
		cos(wz), sin(wz), 0,
		-sin(wz), cos(wz), 0,
		0, 0, 1
		);
}

/***********************/
/* Printing functions. */
/***********************/

void log_debug_matrix(const Matrix4d & mat) {
	log_debug("[\n");
	for (int row = 0; row < DIM; ++row) {
		for (int col = 0; col < DIM; ++col) {
			log_debug_less("%f ", mat.get(row, col));
		}
		log_debug_less("\n");
	}
	log_debug_less("]\n");
}