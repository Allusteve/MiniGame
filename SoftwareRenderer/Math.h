#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Math
{
public:
	Math(){}

	static bool equal(float x, float y)
	{
		return fabs(x - y) <= 1e-6;
	}

	static glm::mat4 GetViewPortMatrix(int ox, int oy, int width, int height) {

		glm::mat4 result = glm::mat4(1.0f);
		result[0][0] = width / 2.0f;
		result[3][0] = ox + (width / 2.0f);
		result[1][1] = height / 2.0f;
		result[3][1] = oy + (height / 2.0f);
		return result;
	}


//观察矩阵
// V = R*T
// T = [  1 , 0 , 0 , -eyex          R = [  Right , 0 
//        0 , 1 , 0 , -eyey                   UP  , 0
//        0 , 0 , 1 , -eyez               - Front , 0
//        0 , 0 , 0 ,   1   ]                 0   , 1 ]
// V =  [  Right  ,  - Right·eye
//          UP   ,  - UP·eye
//       -Front  ,   Front·eye
//         0     ,       1        ]
// 世界空间一般用右手系，靠近屏幕为+Z，从摄像机视角看过去的话+Z是朝向屏幕里的
// 为了让朝向保持不变，对Front向量取反
	static glm::mat4 GetViewMatrix(const glm::vec3& pos, const glm::vec3& front, const glm::vec3& right, const glm::vec3& up)
	{
		glm::mat4 result = glm::mat4(1.0f);
		result[0][0] = right.x;
		result[1][0] = right.y;
		result[2][0] = right.z;
		result[3][0] = -glm::dot(right, pos);
		result[0][1] = up.x;
		result[1][1] = up.y;
		result[2][1] = up.z;
		result[3][1] = -glm::dot(up, pos);
		result[0][2] = -front.x;
		result[1][2] = -front.y;
		result[2][2] = -front.z;
		result[3][2] = glm::dot(front, pos);
		return result;
	}


	//透视投影 参数 fov(弧度) aspect near far
	//M = [   1/aspect*tan(fov/2),       0      ,         0      ,       0
	//               0  ,         1/tan(fov/2)  ,         0      ,       0 
	//               0  ,                0      ,  - (f+n)/(f-n) ,  -2fn/(f-n)
	//               0  ,                0      ,         -1     ,       0     ]
	// 投影之后从右手系变成了左手系，Z靠近屏幕的是负
	static glm::mat4 GetProjectionMatrix(const float fovy, const float aspect, const float n, const float f)
	{
		glm::mat4 result = glm::mat4(0.0f);
		const float tanHalfFov = tan(fovy * 0.5f);
		result[0][0] = 1.0f / (aspect * tanHalfFov);
		result[1][1] = 1.0f / (tanHalfFov);
		result[2][2] = -(f + n) / (f - n);
		result[2][3] = -1.0f;
		result[3][2] = (-2.0f * n * f) / (f - n);

		return result;
	}
	


	static glm::vec4 Lerp(const glm::vec4& v1, const glm::vec4& v2, float factor) {
		return (1.0f - factor) * v1 + factor * v2;
	}

	static glm::vec3 Lerp(const glm::vec3& v1, const glm::vec3& v2, float factor) {
		return (1.0f - factor) * v1 + factor * v2;
	}

	static glm::vec2 Lerp(const glm::vec2& v1, const glm::vec2& v2, float factor) {
		return (1.0f - factor) * v1 + factor * v2;
	}

	static float Lerp(const float& f1, const float& f2, float factor) {
		return (1.0f - factor) * f1 + factor * f2;
	}
	
};
