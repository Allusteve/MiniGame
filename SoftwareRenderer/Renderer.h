#pragma once
#include "FrameBuffer.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"

class Renderer
{
public:
	Renderer(const Renderer& r) = delete;
	void Init(int width, int height);
	void DrawTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2);
	void DrawMesh(const Mesh& mesh);
	void SetViewportMatrix(const glm::mat4& viewport);
	void SetModelMatrix(const glm::mat4& model);
	void SetLookAtMatrix(const glm::mat4& lookat);
	void SetProjectionMatrix(const glm::mat4& projection);
	void LoadTexture(const std::string& path);
	void Flush();

	//扫描线填充算法 Line Sweeping
	//对任意三角形，分为上下两个平底三角形填充
	void ScanLineTriangle(const V2F& v1, const V2F& v2, const V2F& v3);
	void UpTriangle(const V2F& v1, const V2F& v2, const V2F& v3);
	void DownTriangle(const V2F& v1, const V2F& v2, const V2F& v3);
	void ScanLine(const V2F& left, const V2F& right);
	FrameBuffer* GetFramebuffer() const;

	void ClearColorBuffer(const glm::vec4& color);
	void ClearDepthBuffer();

	static Renderer& Get();

private:
	Renderer() {}
	FrameBuffer* m_FrameBuffer = nullptr;
	Shader* m_Shader = nullptr;
	Texture* m_texture0 = nullptr;
	glm::mat4 m_ViewPortMatrix = glm::mat4(1.0f);
	glm::mat4 m_LookAtMatrix = glm::mat4(1.0f);
	glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
	int m_Width = 800;
	int	m_Height = 600;
};