#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(const int& w /*= 800*/, const int& h /*= 600*/)
{
	width = w;
	height = h;
	//默认生成格式为RGBA的帧缓冲
	colorBuffer.resize(width * height * 4, 0);
	depthBuffer.resize(width * height, 1.0f);
}

void FrameBuffer::Resize(const int& w, const int& h)
{
		width = w;
		height = h;
		colorBuffer.resize(width * height * 4);
		depthBuffer.resize(width * height, 1.0f);
}

void FrameBuffer::ClearColorBuffer(const glm::vec4& color)
{
	unsigned char* p = colorBuffer.data();
	for (int i = 0; i < width * height * 4; i += 4) {
		*(p + i) = color.r * 255;
		*(p + i + 1) = color.g * 255;
		*(p + i + 2) = color.b * 255;
		*(p + i + 3) = color.a * 255;
	}
}

void FrameBuffer::ClearDepthBuffer()
{
	auto p = depthBuffer.data();
	for (int i = 0; i < width * height; i++)
	{
		*(p + i) = 1.0f;
	}
}

void FrameBuffer::WritePoint(const int& x, const int& y, const glm::vec4& color)
{
	{
		if (x < 0 || x >= width || y < 0 || y >= height)
		{
			return;
		}

		int idx = x + y * width;

		colorBuffer[4 * idx + 0] = color.r;
		colorBuffer[4 * idx + 1] = color.g;
		colorBuffer[4 * idx + 2] = color.b;
		colorBuffer[4 * idx + 3] = color.a;
	}
}

unsigned char* FrameBuffer::data()
{
	return colorBuffer.data();
}

float FrameBuffer::GetDepth(const int x, const int y)
{
	if (x < 0 || x >= width || y < 0 || y >= height)
		return 1.0;
	return *(depthBuffer.data() + y * width + x);
}

void FrameBuffer::WriteDepth(const int x, const int y, const float depth)
{
	if (x < 0 || x >= width || y < 0 || y >= height)
		return;
	float* p = depthBuffer.data();
	*(p + y * width + x) = depth;
}
