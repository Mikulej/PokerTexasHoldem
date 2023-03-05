#include "Image.h"
std::vector<Image> Image::renderList;
extern int SCR_WIDTH, SCR_HEIGHT;
Image::Image(unsigned int _texture,float _x,float _y,unsigned int _layer) : texture(_texture),layer(_layer){
    Pos = glm::vec4(_x,_y,0, 0);
    Scale = glm::vec4(1,1,1,1); //Scale = glm::vec4(0,0,0,0);
    //Color = glm::vec4(0, 0, 0, 0);
    Offset = glm::vec2(0, 0);
    Zoom = glm::vec2(1, 1);
    angle = 0;
    set_color();
}
Image::~Image() {
   
}
int Image::AddImage(unsigned int texture, float _x, float _y,unsigned int _layer) {
    //zostuj sortowanie na podstawie warstwy
    if (renderList.empty()) { renderList.push_back(Image(texture, _x, _y, _layer)); return 0; }
    int r_index = -1;
    std::vector<Image>::iterator pom;
    pom = std::find_if(renderList.begin(), renderList.end(), [_layer,&r_index](Image &img) -> bool { 
        r_index++;
        return img.layer == _layer;});
    renderList.insert(pom, Image(texture, _x, _y, _layer));//dodajesz na poczatek danego layer kazdy kolejny musi miec dodany indeks +1
    return r_index;
}
/// <summary>
/// Funkcja  (0;255)->(0;1)
/// </summary>
/// <param name="_rgba">Wartosc od 0 do 255</param>
/// <returns></returns>
inline glm::vec4 rgba_to_4f(glm::vec4 &vec) { 
    vec.x /= 255;
    vec.y /= 255;
    vec.z /= 255;
    vec.w /= 255;
    return vec;
}
void Image::set_color(int _c,bool _marked) {
    int r = _c;
    if(_c == -1){r=rand() % 6;}
    glm::vec4 rcolor;
    switch (r) {
    case 0://no color
        rcolor = glm::vec4(255, 255, 255, 255);
        break;
    case 1://mark
        rcolor = glm::vec4(200, 200, 200, 255);
        break; 
    case 2://cyan
        rcolor = glm::vec4(113, 235, 229, 255);
        break;
    case 3://violet
        rcolor = glm::vec4(143, 121, 252, 255);
        break;
    case 4://lime
        rcolor = glm::vec4(174, 243, 89, 255);
        break;
    case 5://candy
        rcolor = glm::vec4(210, 20, 4, 255);
        break;
    case 6://tangerine
        rcolor = glm::vec4(250, 129, 40, 255);
        break;
    case 7://pink
        rcolor = glm::vec4(222, 124, 235, 255);
        break;
    case 8://gray
        rcolor = glm::vec4(105,105,105, 255);
        break;
    case 10://black
        rcolor = glm::vec4(0, 0, 0,255);
        break;
  
    }  
    if (_marked) { rcolor.x -= 55; rcolor.y -= 55; rcolor.z -= 55;  }
    Color = rgba_to_4f(rcolor);
}
void Image::set_scale(float _x,float _y) {
    Scale = glm::vec4(_x, _y, 1, 1);
}
void Image::RenderAll(unsigned int _VAO, Shader& ourShader) { //wywoluje dla kazdego z osobna bazujac na polach obiektu Image
    if (renderList.empty()) { return; }
    //Render
    ourShader.use();
    glBindVertexArray(_VAO);
    //get uniform locations
    unsigned int transLoc = glGetUniformLocation(ourShader.ID, "transform");
    unsigned int colorLoc = glGetUniformLocation(ourShader.ID, "ourColor");
    unsigned int texZoomLoc = glGetUniformLocation(ourShader.ID, "texZoom");
    unsigned int texOffsetLoc = glGetUniformLocation(ourShader.ID, "texOffset");
    
    //testMat[row][col] = blabla; TAK NIE JEST!
    //testMat[col][row] = blabla; TAK CHYBA JEST
    for (auto img : renderList) {
        glm::mat4 trans = glm::mat4(1.0f);
        glm::mat4 temp = glm::mat4(1.0f);
        //Przesun
        temp = glm::mat4(1.0f);
        temp[3][0] = img.Pos.x;
        temp[3][1] = img.Pos.y;
        trans = trans * temp;
        //Obrot
        temp = glm::mat4(1.0f);
        float alfa = img.angle;
        temp[0][0] = cos(alfa);
        temp[0][1] = -sin(alfa);
        temp[1][0] = sin(alfa);
        temp[1][1] = cos(alfa);
        trans = trans * temp;
        //Skaluj
        temp = glm::mat4(1.0f);
        temp[0][0] = img.Scale.x;// / (double)SCR_WIDTH;
        temp[1][1] = img.Scale.y;// / (double)SCR_HEIGHT;
        trans = trans * temp;
        //Zbinduj teksture
        glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
        glBindTexture(GL_TEXTURE_2D, img.texture);
        //Przypisz uniformy
        glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(trans));
        glUniform4f(colorLoc, img.Color.x, img.Color.y, img.Color.z, img.Color.w);
        glUniform2f(texOffsetLoc, img.Offset.x, img.Offset.y);
        glUniform2f(texZoomLoc, img.Zoom.x, img.Zoom.y);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);    
    }
}