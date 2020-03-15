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
	void ClearDepthBuffer();

	// vector下标访问在Debug状态下，会进行安全检查，效率较低
	void WritePoint(const int& x, const int& y, const glm::vec4& color);


	unsigned char* data();

	float GetDepth(const int x, const int y);

	void WriteDepth(const int x, const int y, const float depth);


private:
	std::vector<unsigned char> colorBuffer;
	std::vector<float> depthBuffer;
	int width;
	int height;
};