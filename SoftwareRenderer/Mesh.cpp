#include "Mesh.h"
#include <iostream>

Mesh::Mesh(const int vboNumber, const int iboNumber)
{
	VertexBuffer.resize(vboNumber);
	IndexBuffer.resize(iboNumber);
}

Mesh::Mesh(const Mesh& mesh)
	:VertexBuffer(mesh.VertexBuffer), IndexBuffer(mesh.IndexBuffer)
{
}


Mesh& Mesh::operator +=(const Mesh& mesh)
{
	AddMesh(mesh);
	return *this;
}

Mesh& Mesh::operator =(const Mesh& mesh)
{
	if (&mesh == this)
	{
		return *this;
	}

	this->IndexBuffer = mesh.IndexBuffer;
	this->VertexBuffer = mesh.VertexBuffer;

	return *this;
}

void Mesh::AddMesh(const Mesh& mesh)
{

	int offset = VertexBuffer.size();
	VertexBuffer.insert(VertexBuffer.end(), mesh.VertexBuffer.begin(), mesh.VertexBuffer.end());
	IndexBuffer.reserve(IndexBuffer.size() + mesh.IndexBuffer.size());

	for (int i = 0; i < mesh.IndexBuffer.size(); i++)
	{
		IndexBuffer.push_back(offset + mesh.IndexBuffer[i]);
		std::cout << offset + mesh.IndexBuffer[i] << std::endl;
	}
	std::cout << "VBO size: " << VertexBuffer.size() << " IBO size: " << IndexBuffer.size() << std::endl;
	std::cout << "--------------------" << std::endl;

}

void Mesh::AddTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3)
{
	VertexBuffer.push_back(v1);
	VertexBuffer.push_back(v2);
	VertexBuffer.push_back(v3);

	int offset = VertexBuffer.size();
	IndexBuffer.push_back(offset);
	IndexBuffer.push_back(offset + 1);
	IndexBuffer.push_back(offset + 2);
}
