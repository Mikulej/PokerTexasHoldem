#ifndef IMAGE_H
#define IMAGE_H
#include <ctime>
#include <cstdlib>
#include <glad/glad.h>
#include <iostream>
#include <cmath>
#include <ctime>
#include <vector>
#include <algorithm>
#include "shader.h"
#ifndef STBI
#define STBI
#include "stb_image.h"
#endif
#include "glm-master/glm/glm.hpp"
#include "glm-master/glm/gtc/matrix_transform.hpp"
#include "glm-master/glm/gtc/type_ptr.hpp"
#include <GLFW/glfw3.h>
#define PI 3.14159265358979323846 
class Image {
	static std::vector<Image> renderList;
public:
	glm::vec4 Pos;
	glm::vec4 Scale;
	glm::vec4 Color;
	glm::vec2 Offset;
	glm::vec2 Zoom;
	float angle;
	unsigned int layer;
	unsigned int texture;
	int height;
	int width;
	Image(unsigned int _texture,float _x = 0, float _y = 0, unsigned int _layer = 0);
	~Image();
	static void RenderAll(unsigned int _VAO, Shader& ourShader);
	static int AddImage(unsigned int texture, float _x=0, float _y=0,unsigned int _layer=0);
	friend class StaticObject;
	//MATRIX MAINPULATION
	void set_color(int _c = -1,bool _marked = false);
	void set_scale(float _x,float _y);
};
#endif