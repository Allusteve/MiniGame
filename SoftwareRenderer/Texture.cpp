#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const std::string& path)
{
	data = stbi_load(path.c_str(), &width, &height, &channels, 0);
}

Texture::~Texture()
{
	if (data != nullptr)
		free(data);
}

void Texture::LoadTexture(const std::string& path)
{
	if (data != nullptr)
		free(data);

	data = stbi_load(path.c_str(), &width, &height, &channels, 0);
}

glm::vec4 Texture::Sample2D(const glm::vec2& texcoords)
{
	// GL_REPEAT
	float x = texcoords.x - (int)texcoords.x;
	float y = texcoords.y - (int)texcoords.y;

	return GetColor(x * width, y * height);
}

glm::vec4 Texture::GetColor(int x, int y)
{
	if (x < 0 || x >= width || y < 0 || y >= height)
	{
		return glm::vec4(0.0f);
	}

	int offset = channels * (x + y * width);
	return glm::vec4(*(data + offset), *(data + offset + 1), *(data + offset + 2), *(data + offset + 3));
}
