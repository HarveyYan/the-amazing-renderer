#include <limits>
#include <algorithm>
#include "myHelpers.h"
#include <vector>

#include <iostream>

#define TOLERANCE 0.00001

/*
Compare for equality between two double floating point numbers, taking into account that small differences are possible
due to percision errors.
*/
bool almost_eq(double lhs, double rhs) {
	return lhs == rhs || std::abs(lhs - rhs) < (std::min)(std::abs(rhs), std::abs(lhs)) * TOLERANCE;
}

void basic_line_draw(CDC * pDC, int x1, int y1, int x2, int y2, COLORREF c) {
	double dx = x2 - x1;
	double dy = y2 - y1;

	if (x1 == x2) {
		for (double y = y1; y < y2; y++) {
			pDC->SetPixel(x1, y, c);
		}
	}
	else if (y1 == y2) {
		for (double x = x1; x < x2; x++) {
			pDC->SetPixel(x, y1, c);
		}
	}
	else {
		double slope = dy / dx;
		double y = y1;
		for (int x = x1; x <= x2; x++) {
			pDC->SetPixel(x, std::round(y), c);
			y = y + slope;
		}
	}
}//

void swap(int *x, int *y) {
	int temp;
	temp = *x;
	*x = *y;
	*y = temp;
}

// Line drawing using the midpoint algorithm with floating point arithmetic.
void midpoint_line_draw(CDC* pDC, int x1, int y1, int x2, int y2, COLORREF c){
	double x, y;
	double dx = x2 - x1;
	double dy = y2 - y1;

	pDC->SetPixel(x1, y1, c);
	pDC->SetPixel(x2, y2, c);

	double slope = dy / dx;
	double slopeinv = dx / dy;

	if (slope < 1.0 && slope > -1.0) {
		if (x1 > x2) {
			// Make sure first point is to the left.
			swap(&x1, &x2);
			swap(&y1, &y2);
		}
		if (y1 < y2) { // Draw from top to bottom (y axis points down).
			x = x1;
			y = y1;
			double error = slope - 0.5;
			while (x < x2) {
				x++;
				if (error > 0) {
					error += slope - 1;
					y++;
				}
				else {
					error += slope;
				}

				pDC->SetPixel(x, y, c);
			}
		}
		else { // Draw from right to left.
			x = x2;
			y = y2;
			double error = -slope - 0.5;
			while (x > x1) {
				x--;
				if (error > 0) {
					error += -slope - 1;
					y++;
				}
				else {
					error += -slope;
				}
				pDC->SetPixel(x, y, c);
			}
		}
	}
	else {
		if (y1 > y2){
			swap(&x1, &x2);
			swap(&y1, &y2);
		}

		if (x1 < x2) { // Draw from top to bottom.
			x = x1;
			y = y1;
			double error = slopeinv - 1;
			while (y <y2) {
				++y;
				if (error > 0) {
					error += slopeinv - 1;
					x++;
				}
				else {
					error += slopeinv;
				}
				pDC->SetPixel(x, y, c);
			}
		}
		else{ // Draw from bottom to top.
			x = x2;
			y = y2;
			double error = -slopeinv - 1;
			while (y > y1) {
				--y;
				if (error > 0) {
					error += -slopeinv - 1;
					x++;
				}
				else {
					error += -slopeinv;
				}
				pDC->SetPixel(x, y, c);
			}
		}
	}
}

void flood_fill(CDC * pDC, int x, int y, COLORREF c, COLORREF bgColor) {
	COLORREF pixelColor = pDC->GetPixel(x, y);
	if (pixelColor != bgColor) {
		return;
	}
	pDC->SetPixel(x, y, c);
	flood_fill(pDC, x + 1, y, c, bgColor);
	flood_fill(pDC, x, y + 1, c, bgColor);
	flood_fill(pDC, x - 1, y, c, bgColor);
	flood_fill(pDC, x, y - 1, c, bgColor);
}

void paint_hack(CDC * pDC, int width, int height, COLORREF c, COLORREF bgColor) {
	for (int row = 0; row < height; ++row) {
		bool colorPixel = false;
		
		int nIntesections = 0;
		for (int col = 0; col < width; ++col) {
			COLORREF currentColor = pDC->GetPixel(col, row);
			if (currentColor != bgColor) {
				colorPixel = !colorPixel;
				nIntesections++;
			}
			while (currentColor != bgColor && col < width) {
				col++;
				currentColor = pDC->GetPixel(col, row);
			}
		}
		if (nIntesections < 2)
			continue;

		for (int col = 0; col < width; ++col) {
			COLORREF currentColor = pDC->GetPixel(col, row);
			if (currentColor != bgColor) {
				colorPixel = !colorPixel;
			}
			while (currentColor != bgColor && col < width) {
				col++;
				currentColor = pDC->GetPixel(col, row);
			}

			if (colorPixel) {
				if (pDC->SetPixel(col, row, c) < 0) {
				}
			}
		}
	}
}