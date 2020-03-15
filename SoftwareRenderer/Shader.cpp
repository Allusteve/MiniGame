#include "Shader.h"


Shader::Shader() {}



V2F Shader::VertexShader(const Vertex& a2v)
{
	V2F o;
	o.worldPos = ModelMatrix * a2v.position;
	o.windowPos = ProjectMatrix * ViewMatrix * o.worldPos;
	//投影之后w=-z
	o.Z = 1 / o.windowPos.w;
	o.worldPos *= o.Z;
	o.color = a2v.color * o.Z;
	//同一个面法线是一样的 不用插值
	o.normal = a2v.normal;
	o.texcoord = a2v.texCoords * o.Z;
	return o;
}

glm::vec4 Shader::FragmentShader(const V2F& v)
{
	// Write Shader file here
	
	glm::vec4 color = texture_unit0->Sample2D(v.texcoord);
	return color;
}

void Shader::setModelMatrix(const glm::mat4& model)
{
	ModelMatrix = model;
}

void Shader::setViewMatrix(const glm::mat4& view)
{
	ViewMatrix = view;
}

void Shader::setProjectMatrix(const glm::mat4& project)
{
	ProjectMatrix = project;
}

void Shader::setShaderTexture_unit0(Texture* tex)
{
	texture_unit0 = tex;
}

