#include "StaticObject.h"
unsigned int texture_box,texture_karty;// czy tu powinny byc ID tekstur?
extern double mx, my;
extern int SCR_WIDTH, SCR_HEIGHT;
std::vector<StaticObject> StaticObject::collisionList;
std::vector<StaticObject>::iterator StaticObject::Add(unsigned int _texture, float _x, float _y, unsigned int _layer) {
	StaticObject::collisionList.push_back(StaticObject(_texture, _x, _y,_layer));
	std::vector<StaticObject>::iterator pom = collisionList.end() - 1;
	(*pom).img_id = Image::AddImage(_texture, _x, _y, _layer);//alokuj obrazek
	//std::cout << "Creating ID: " << (*pom).img_id << std::endl;
	//wszyscy co mieli index wiekszy img_id musza swoj index zwiekszyc o 1
	std::for_each(StaticObject::collisionList.begin(), StaticObject::collisionList.end(), [=](StaticObject& so) {
		if (so.img_id >= (*pom).img_id) { so.add_to_index(1); } });
	(*pom).img_id--;
	return pom;
}
void StaticObject::AddItem(int _ID,float _x,float _y,int a,int b) {
	std::vector<StaticObject>::iterator pom;
	switch (_ID) {
	default:
		std::cout << " Wrong ID for creating StaticObject - void StaticObject::AddItem(int _ID,float _x,float _y) " << std::endl;
		break;
	case 0://regular button
		pom = Add(texture_box, _x, _y, 0);
		(*pom).set_color(8); (*pom).original_color = 8;
		(*pom).set_scale(6.0f,2.0f);
		break;
	case 1://arrow button
		pom = Add(texture_box, _x, _y, 0);
		(*pom).set_color(8); (*pom).original_color = 8;
		(*pom).set_scale(1.0f, 1.0f);
		break;
	case 2://small button
		pom = Add(texture_box, _x, _y, 0);
		(*pom).set_color(8); (*pom).original_color = 8;
		(*pom).set_scale(4.0f, 1.0f);
		break;
	case 3://text box
		pom = Add(texture_box, _x, _y, 0);
		(*pom).set_color(8); (*pom).original_color = 8;
		(*pom).set_scale(24.0f, 12.0f);
		break;
	case 10://card reverse
		pom = Add(texture_karty, _x, _y, 0);
		(*pom).set_color(0); (*pom).original_color = 0;
		(*pom).set_scale(7.1f/2.0f,9.6f/2.0f);
		(*pom).set_offset(Karta::get_rewers(), 0);
		(*pom).set_zoom(13.0f, 5.0f);
		break;
	case 11://card 
		pom = Add(texture_karty, _x, _y, 0);
		(*pom).set_color(0); (*pom).original_color = 0;
		(*pom).set_scale(7.1f / 2.0f, 9.6f / 2.0f);
		(*pom).set_offset(a-1, 4-b);
		(*pom).set_zoom(13.0f, 5.0f);
		break;
	}

}
void StaticObject::DelLast(void) {
	if (collisionList.empty()) { return; }
	std::vector<StaticObject>::iterator pom = collisionList.end() - 1;
	//std::cout << "Deleting ID: " << (*pom).img_id << std::endl;
	Image::renderList.erase(std::next(Image::renderList.begin(), (*pom).img_id));//usuwa obrazek z renderList
	//wszyscy co mieli index wiekszy img_id musza swoj index zmniejszyc o 1
	std::for_each(StaticObject::collisionList.begin(), StaticObject::collisionList.end(), [=](StaticObject& so) {
		if (so.img_id >= (*pom).img_id) { so.add_to_index(-1); } });
	StaticObject::collisionList.pop_back();
}
void StaticObject::DelAll(void) {
	while (!collisionList.empty()) { DelLast(); }
}
StaticObject::StaticObject(unsigned int _texture,float _x,float _y, unsigned int _layer):img_id(-1), x(_x),y(_y) {
}
StaticObject::~StaticObject() {
}
StaticObject::StaticObject(const StaticObject& so) : x(so.x),y(so.y),lifetime(so.lifetime) {
}
void StaticObject::set_pos(float _x, float _y) {
	Image::renderList[img_id].Pos.x = x = _x; Image::renderList[img_id].Pos.y = y = _y;
}
void StaticObject::add_pos(float _x, float _y) {
	Image::renderList[img_id].Pos.x = x += _x; Image::renderList[img_id].Pos.y = y += _y;
}
void StaticObject::add_to_index(int i) {
	img_id += i;
}
void StaticObject::set_color(int _c,bool _marked) {
	Image::renderList[img_id].set_color(_c,_marked);
}
void StaticObject::set_scale(float _x,float _y) {//docelowo 16:9
	Image::renderList[img_id].Scale = glm::vec4(_x/16.0f, _y/9.0f, 1, 1);
}
void StaticObject::add_offset(float _x, float _y) {
	Image::renderList[img_id].Offset += glm::vec2(_x, _y);
}
void StaticObject::set_offset(float _x, float _y) {
	Image::renderList[img_id].Offset = glm::vec2(_x, _y);
}
void StaticObject::set_zoom(float _x, float _y) {
	Image::renderList[img_id].Zoom = glm::vec2(_x, _y);
}
void StaticObject::set_texture(unsigned int _texture) {
	Image::renderList[img_id].texture = _texture;
}
bool StaticObject::pointed_by_mouse() {
	float width = 0.5f * (float)Image::renderList[img_id].Scale.x;
	float hight = 0.5f* (float)Image::renderList[img_id].Scale.y;
	if (mx > x - width && mx < x + width && my < y + hight && my > y - hight) { //pamietaj o uwzglednieniu rozciagajacego sie okna!
		set_color(original_color,true);
		return true;
	}
	set_color(original_color);
	return false;
}
float StaticObject::get_text_x(float text_width) {	//(-1;1)->(0;SCR_WIDTH)
	return ((x+ 1 ) * (float)SCR_WIDTH - text_width) / 2.0f ;
}
float StaticObject::get_text_y(float text_height) {
	return  ((y + 1) * (float)SCR_HEIGHT - text_height)/ 2.0f;
}