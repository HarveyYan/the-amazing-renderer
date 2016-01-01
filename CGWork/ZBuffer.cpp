#include "ZBuffer.h"
#include <limits>


ZBuffer::ZBuffer(int w, int h)
{
	allocate(w, h);
}

ZBuffer::~ZBuffer()
{
	free();
}

void ZBuffer::resize(int w, int h) {
	free();
	allocate(w, h);
}

void ZBuffer::set(int x, int y, double z) {
	buffer[y][x] = z;
}

int ZBuffer::get(int x, int y) const {
	return buffer[y][x];
}

void ZBuffer::free() {
	for (int r = 0; r < height; ++r) {
		delete[] buffer[r];
	}
	delete[] buffer;
}

void ZBuffer::allocate(int w, int h) {
	//TODO allocate as one contigious memory block of size h*w. The current way is not very efficient.
	buffer = new double*[h];
	for (int r = 0; r < h; ++r) {
		buffer[r] = new double[w];
		for (int c = 0; c < w; ++c) {
			buffer[r][c] = std::numeric_limits<double>::lowest(); // set to be as far away as possible.
		}
	}
	width = w;
	height = h;
}