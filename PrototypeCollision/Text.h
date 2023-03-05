#pragma once
#ifndef TEXT_H
#define TEXT_H


#include <vector>
#include <string>
#include <map>
#include "shader.h"
#include "StaticObject.h"

#include "glm-master/glm/glm.hpp"
#include "glm-master/glm/gtc/matrix_transform.hpp"
#include "glm-master/glm/gtc/type_ptr.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

class Text {
	static unsigned int VAO, VBO;
	static std::vector<Text> textList;
	std::string text;
	float scalex;
	float scaley;
	glm::vec3 color;
	int ID; //index na powizany obiekt
	float x, y;
	float width, height;
	bool raw;
	bool fixed;
	bool clickable;
	Text(std::string _text, float _scalex, float _scaley, glm::vec3 _color, int _ID, bool _clickable);
	Text(std::string _text, float x, float y, float _scalex, float _scaley, glm::vec3 _color,bool _fixed = false);
	float getTextWidth(std::string text, float _scalex);
	float getTextHeight(std::string text, float _scaley);
	void RenderText(Shader& shader, std::string text, float x, float y, float _scalex, float _scaley, glm::vec3 color);
public:
	static void RenderAll(Shader& shader);
	static void Add(std::string _text, float _scalex, float _scaley, glm::vec3 _color, int _ID, bool _clickable = false);
	static void AddRaw(std::string _text, float x, float y, float _scalex, float _scaley, glm::vec3 _color, bool _fixed=false);
	static void DelAll();
	static void ftInit();
};
#endif 