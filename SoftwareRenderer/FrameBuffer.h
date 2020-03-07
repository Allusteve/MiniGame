#pragma once
#include <vector>
#include "glm/glm.hpp"

class FrameBuffer
{
public:
	FrameBuffer(const int& w = 800, const int& h = 600);

	~FrameBuffer() = default;

	void Resize(const int& w, const int& h);


	void ClearColorBuffer(const glm::vec4& color);


	// vector下标访问在Debug状态下，会进行安全检查，效率较低
	void WritePoint(const int& x, const int& y, const glm::vec4& color);


	unsigned char* data();


private:
	std::vector<unsigned char> colorBuffer;
	int width;
	int height;
};