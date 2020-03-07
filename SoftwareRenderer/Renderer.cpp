#include "Renderer.h"
#include <glad/glad.h>



Renderer& Renderer::Get()
{
	static Renderer instance;
	return instance;
}

void Renderer::Init(int width, int height)
{
	m_FrameBuffer = new FrameBuffer(width, height);
	m_Width = width;
	m_Height = height;
	m_Shader = new Shader();
}

void Renderer::DrawTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
{
	V2F o1 = m_Shader->VertexShader(v0);
	V2F o2 = m_Shader->VertexShader(v1);
	V2F o3 = m_Shader->VertexShader(v2);

	o1.windowPos = m_ViewPortMatrix * o1.windowPos;
	o2.windowPos = m_ViewPortMatrix * o2.windowPos;
	o3.windowPos = m_ViewPortMatrix * o3.windowPos;
	ScanLineTriangle(o1, o2, o3);
}

void Renderer::SetViewportMatrix(const glm::mat4& viewport)
{
	m_ViewPortMatrix = viewport;
}

void Renderer::Flush()
{
	glDrawPixels(m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, m_FrameBuffer->data());
}

void Renderer::ScanLineTriangle(const V2F& v1, const V2F& v2, const V2F& v3)
{
	std::vector<V2F> arr = { v1,v2,v3 };
	if (arr[0].windowPos.y > arr[1].windowPos.y) {
		V2F tmp = arr[0];
		arr[0] = arr[1];
		arr[1] = tmp;
	}
	if (arr[1].windowPos.y > arr[2].windowPos.y) {
		V2F tmp = arr[1];
		arr[1] = arr[2];
		arr[2] = tmp;
	}
	if (arr[0].windowPos.y > arr[1].windowPos.y) {
		V2F tmp = arr[0];
		arr[0] = arr[1];
		arr[1] = tmp;
	}
	//arr[0] 在最下面  arr[2]在最上面
	//中间跟上面的相等，是底三角形
	if (Math::equal(arr[1].windowPos.y, arr[2].windowPos.y)) {
		DownTriangle(arr[1], arr[2], arr[0]);
	}//顶三角形
	else if (Math::equal(arr[1].windowPos.y, arr[0].windowPos.y)) {
		UpTriangle(arr[1], arr[0], arr[2]);
	}
	else {
		//插值求出中间点对面的那个点，划分为两个新的三角形
		float weight = (arr[2].windowPos.y - arr[1].windowPos.y) / (arr[2].windowPos.y - arr[0].windowPos.y);
		V2F newEdge = V2F::lerp(arr[2], arr[0], weight);
		UpTriangle(arr[1], newEdge, arr[2]);
		DownTriangle(arr[1], newEdge, arr[0]);
	}
}

void Renderer::UpTriangle(const V2F& v1, const V2F& v2, const V2F& v3)
{
	V2F left, right, top;
	left = v1.windowPos.x > v2.windowPos.x ? v2 : v1;
	right = v1.windowPos.x > v2.windowPos.x ? v1 : v2;
	top = v3;
	left.windowPos.x = int(left.windowPos.x);
	int dy = top.windowPos.y - left.windowPos.y;
	int nowY = top.windowPos.y;
	//从上往下插值
	for (int i = dy; i >= 0; i--) {
		float weight = 0;
		if (dy != 0) {
			weight = (float)i / dy;
		}
		V2F newLeft = V2F::lerp(left, top, weight);
		V2F newRight = V2F::lerp(right, top, weight);
		newLeft.windowPos.x = int(newLeft.windowPos.x);
		newRight.windowPos.x = int(newRight.windowPos.x + 0.5);
		newLeft.windowPos.y = newRight.windowPos.y = nowY;
		ScanLine(newLeft, newRight);
		nowY--;
	}
}

void Renderer::DownTriangle(const V2F& v1, const V2F& v2, const V2F& v3)
{
	V2F left, right, bottom;
	left = v1.windowPos.x > v2.windowPos.x ? v2 : v1;
	right = v1.windowPos.x > v2.windowPos.x ? v1 : v2;
	bottom = v3;
	int dy = left.windowPos.y - bottom.windowPos.y;
	int nowY = left.windowPos.y;
	//从上往下插值
	for (int i = 0; i < dy; i++) {
		float weight = 0;
		if (dy != 0) {
			weight = (float)i / dy;
		}
		V2F newLeft = V2F::lerp(left, bottom, weight);
		V2F newRight = V2F::lerp(right, bottom, weight);
		newLeft.windowPos.x = int(newLeft.windowPos.x);
		newRight.windowPos.x = int(newRight.windowPos.x + 0.5);
		newLeft.windowPos.y = newRight.windowPos.y = nowY;
		ScanLine(newLeft, newRight);
		nowY--;
	}
}

void Renderer::ScanLine(const V2F& left, const V2F& right)
{
	int length = right.windowPos.x - left.windowPos.x;
	for (int i = 0; i < length; i++) {
		V2F v = V2F::lerp(left, right, (float)i / length);
		v.windowPos.x = left.windowPos.x + i;
		v.windowPos.y = left.windowPos.y;
		m_FrameBuffer->WritePoint(v.windowPos.x, v.windowPos.y, m_Shader->FragmentShader(v));
	}
}

FrameBuffer* Renderer::GetFramebuffer() const
{
	return m_FrameBuffer;
}

void Renderer::ClearColorBuffer(const glm::vec4& color)
{
	m_FrameBuffer->ClearColorBuffer(color);
}

