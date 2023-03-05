#pragma once
#include "Image.h"
#include "PokerEngine.h"
#include <iterator>
#include <algorithm>
class StaticObject {
	float x, y;
	float lifetime;
	int img_id; // index renderList na odpowiadajacy obiektowi obrazek
	int original_color;
public:
	static std::vector<StaticObject> collisionList;
	StaticObject(unsigned int, float _x = 0, float _y = 0, unsigned int _layer = 0);
	StaticObject(const StaticObject& so);
	~StaticObject();
	static std::vector<StaticObject>::iterator Add(unsigned int, float _x = 0, float _y = 0, unsigned int _layer = 0);
	static void AddItem(int _ID, float _x = 0, float _y = 0,int a = 0,int b = 0);
	static void DelLast(void);
	static void DelAll(void);
	void set_pos(float, float);
	void add_pos(float, float);
	void add_to_index(int);
	void set_color(int _c = -1,bool _marked=false);
	void set_scale(float _x,float _y);
	void add_offset(float _x, float _y);
	void set_offset(float _x, float _y);
	void set_zoom(float _x, float _y);
	void set_texture(unsigned int _texture);
	bool pointed_by_mouse();
	float get_text_x(float text_width);
	float get_text_y(float text_height);
};