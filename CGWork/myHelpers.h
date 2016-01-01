#pragma once

#include "StdAfx.h"
#include <conio.h>

#define MY_DEBUG // Uncomment to print debug messages.

#ifdef MY_DEBUG
	#define log_debug(M, ...) _cprintf("[DEBUG] %s:%d " M, __FILE__, __LINE__, ##__VA_ARGS__)
	#define log_debug_less(M, ...) _cprintf(M, ##__VA_ARGS__)
#else
	#define log_debug(M, ...)
	#define log_debug_less(M, ...)
#endif

#define log_info(M, ...) _cprintf("[INFO] %s:%d " M, __FILE__, __LINE__, ##__VA_ARGS__)
#define log_warning(M, ...) _cprintf("[WARNING] %s:%d " M, __FILE__, __LINE__, ##__VA_ARGS__)
#define log_error(M, ...) _cprintf("[ERROR] %s:%d " M, __FILE__, __LINE__, ##__VA_ARGS__)
#define log_short(M, ...) _cprintf("[S] " M, ##__VA_ARGS__)

// define some colors

bool almost_eq(double lhs, double rhs);

void basic_line_draw(CDC * pDC, int x1, int y1, int x2, int y2, COLORREF c);
void midpoint_line_draw(CDC * pDC, int x1, int y1, int x2, int y2, COLORREF c);
void flood_fill(CDC * pDC, int x, int y, COLORREF c, COLORREF bgColor);
void paint_hack(CDC * pDC, int width, int height, COLORREF c, COLORREF bgColor);