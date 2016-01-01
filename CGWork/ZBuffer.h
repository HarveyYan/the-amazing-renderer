#pragma once

class ZBuffer
{
public:
	ZBuffer(int w, int h);
	~ZBuffer();

	void resize(int w, int h);
	void set(int x, int y, double z);
	int get(int x, int y) const;
	
private:
	double **buffer;
	int width, height;
	void free();
	void allocate(int w, int h);
};

