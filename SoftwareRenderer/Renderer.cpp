#include "Renderer.h"
#include <glad/glad.h>
#include <cassert>
#include <iostream>

static void PerspectiveDivision(V2F& v)
{
	v.windowPos /= v.windowPos.w;
	v.windowPos.w = 1.0f;
	// OpenGL的Z-Buffer是 [0,1]，而透视除法之后Z除了深度测试已经没用了
	v.windowPos.z = (v.windowPos.z + 1.0) * 0.5;
}

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

void Renderer::DrawMesh(const Mesh& mesh)
{
	assert(!mesh.IndexBuffer.empty());

	for (int i = 0; i < mesh.IndexBuffer.size(); i += 3)
	{
		int index1 = mesh.IndexBuffer[i];
		int index2 = mesh.IndexBuffer[i+1];
		int index3 = mesh.IndexBuffer[i+2];


		Vertex p1 = mesh.VertexBuffer[mesh.IndexBuffer[i]];
		Vertex p2 = mesh.VertexBuffer[mesh.IndexBuffer[i+1]];
		Vertex p3 = mesh.VertexBuffer[mesh.IndexBuffer[i+2]];

		V2F v1 = m_Shader->VertexShader(p1);
		V2F v2 = m_Shader->VertexShader(p2);
		V2F v3 = m_Shader->VertexShader(p3);

		//执行透视除法，变换到NDC
		PerspectiveDivision(v1);
		PerspectiveDivision(v2);
		PerspectiveDivision(v3);


		v1.windowPos = m_ViewPortMatrix * v1.windowPos;
		v2.windowPos = m_ViewPortMatrix * v2.windowPos;
		v3.windowPos = m_ViewPortMatrix * v3.windowPos;

		
		ScanLineTriangle(v1, v2, v3);
	}

}

void Renderer::SetViewportMatrix(const glm::mat4& viewport)
{
	m_ViewPortMatrix = viewport;
}

void Renderer::SetModelMatrix(const glm::mat4& model)
{
	m_Shader->setModelMatrix(model);
}

void Renderer::SetLookAtMatrix(const glm::mat4& lookat)
{
	m_Shader->setViewMatrix(lookat);
}

void Renderer::SetProjectionMatrix(const glm::mat4& projection)
{
	m_Shader->setProjectMatrix(projection);
}

void Renderer::LoadTexture(const std::string& path)
{
	m_texture0 = new Texture(path);
	m_Shader->setShaderTexture_unit0(m_texture0);
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
		//深度测试
		float depth = m_FrameBuffer->GetDepth(v.windowPos.x, v.windowPos.y);
		if (v.windowPos.z < depth) {

			float z = v.Z;
			v.worldPos /= z;
			v.texcoord /= z;
			v.color /= z;

			m_FrameBuffer->WritePoint(v.windowPos.x, v.windowPos.y, m_Shader->FragmentShader(v));
			m_FrameBuffer->WriteDepth(v.windowPos.x, v.windowPos.y, v.windowPos.z);
		}
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

void Renderer::ClearDepthBuffer()
{
	m_FrameBuffer->ClearDepthBuffer();
}	
