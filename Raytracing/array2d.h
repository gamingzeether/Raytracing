#pragma once

template<typename T>
struct array2d {
public:
	array2d(int height, int width) : h(height), w(width), array(new T[height * width]) {}

	int height() const { return h; };
	int width() const { return w; };

	inline T get(int x, int y) const { return array[y * w + x]; };
	inline void set(int x, int y, T val) { array[y * w + x] = val; };
	inline T* data() const { return array; };
protected:
	int h;
	int w;
	T* array;
};

