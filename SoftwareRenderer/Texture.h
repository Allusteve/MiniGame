#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



class Texture
{
public:	
	Texture() = default;
	Texture(const std::string& path);
	~Texture();
	void LoadTexture(const std::string& path);
	glm::vec4 Sample2D(const glm::vec2& texcoords);

private:
	glm::vec4 GetColor(int x, int y);
private:
	int width;
	int height;
	int channels;
	unsigned char* data=nullptr;
	
};