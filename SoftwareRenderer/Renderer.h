#pragma once
#include "FrameBuffer.h"
#include "Shader.h"

class Renderer
{
public:
	Renderer(const Renderer& r) = delete;
	void Init(int width, int height);
	void DrawTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2);
	void SetViewportMatrix(const glm::mat4& viewport);
	void Flush();

	//扫描线填充算法 Line Sweeping
	//对任意三角形，分为上下两个平底三角形填充
	void ScanLineTriangle(const V2F& v1, const V2F& v2, const V2F& v3);
	void UpTriangle(const V2F& v1, const V2F& v2, const V2F& v3);
	void DownTriangle(const V2F& v1, const V2F& v2, const V2F& v3);
	void ScanLine(const V2F& left, const V2F& right);
	FrameBuffer* GetFramebuffer() const;

	void ClearColorBuffer(const glm::vec4& color);

	static Renderer& Get();

private:
	Renderer() {}
	FrameBuffer* m_FrameBuffer = nullptr;
	Shader* m_Shader = nullptr;
	glm::mat4 m_ViewPortMatrix = glm::mat4(1.0f);
	int m_Width, m_Height;
};