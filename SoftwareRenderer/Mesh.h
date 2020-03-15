#pragma once
#include <vector>
#include "Shader.h"

//���������ڱ���ģ�͵����ݽṹ, �����������ݺ�����
class Mesh
{
public:
	Mesh() = default;
	~Mesh() = default;
	Mesh(const int vboNumber, const int iboNumber);
	Mesh(const Mesh& mesh);
	Mesh& operator =(const Mesh& mesh);
	Mesh& operator += (const Mesh& mesh);
	void AddMesh(const Mesh& mesh);
	void AddTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3);

	std::vector<Vertex> VertexBuffer;
	std::vector<uint32_t> IndexBuffer;
};