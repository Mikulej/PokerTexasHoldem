#include "Text.h"
struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2 Size; // Size of glyph
    glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
    unsigned int Advance; // Offset to advance to next glyph
};
std::map<char, Character> Characters;
extern int SCR_WIDTH, SCR_HEIGHT;
std::vector<Text> Text::textList;
unsigned int Text::VAO;
unsigned int Text::VBO;
Text::Text(std::string _text, float _scalex, float _scaley, glm::vec3 _color, int _ID, bool _clickable):text(_text),scalex(_scalex),scaley(_scaley),color(_color),ID(_ID),clickable(_clickable),raw(false) {
    width = getTextWidth(_text, _scalex);
    height = getTextHeight(_text, scaley);
}
Text::Text(std::string _text, float _x, float _y, float _scalex, float _scaley, glm::vec3 _color, bool _fixed) : text(_text),x(_x),y(_y), scalex(_scalex), scaley(_scaley), color(_color), ID(-1), clickable(false), raw(true),fixed(_fixed) {
    width = getTextWidth(_text, _scalex);
    height = getTextHeight(_text, scaley);
}
void Text::Add(std::string _text, float _scalex, float _scaley, glm::vec3 _color, int _ID, bool _clickable) {
	textList.push_back(Text(_text, _scalex, _scaley, _color, _ID, _clickable));
}
void Text::AddRaw(std::string _text, float x, float y, float _scalex, float _scaley, glm::vec3 _color,bool _fixed) {
    textList.push_back(Text(_text,x,y,_scalex,_scaley,_color,_fixed));
}
void Text::DelAll() {
	textList.clear();
}
void Text::RenderAll(Shader& shader) {
	for (auto t : textList) {
        t.width = t.getTextWidth(t.text, t.scalex);
        t.height = t.getTextHeight(t.text, t.scaley);
        if (t.raw) {
            if (t.fixed) {
                t.RenderText(shader, t.text, ((t.x + 1) * (float)SCR_WIDTH) / 2.0f, ((t.y + 1) * (float)SCR_HEIGHT) / 2.0f, t.scalex, t.scaley, t.color);
            }
            else {
                t.RenderText(shader, t.text, ((t.x + 1) * (float)SCR_WIDTH - t.width) / 2.0f, ((t.y + 1) * (float)SCR_HEIGHT - t.height) / 2.0f, t.scalex, t.scaley, t.color);
            }            
        }
        else {
            if (StaticObject::collisionList.size() <= t.ID) { return; }
            glm::vec3 temp_color = t.color;
            if (t.clickable && StaticObject::collisionList.at(t.ID).pointed_by_mouse()) { temp_color.x -= 55 / 255.0f; temp_color.y -= 55 / 255.0f; temp_color.z -= 55 / 255.0f; }
            t.RenderText(shader, t.text, StaticObject::collisionList.at(t.ID).get_text_x(t.width), StaticObject::collisionList.at(t.ID).get_text_y(t.height), t.scalex, t.scaley, temp_color);
        }	
	}
}
float Text::getTextWidth(std::string text, float _scalex) {
    float scalex = (float)SCR_WIDTH * (float)_scalex;
    float width = 0;
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = Characters[*c];
        width += ch.Bearing.x * scalex;
        width += (ch.Advance >> 6) * scalex;
    }
    return width;
}
float Text::getTextHeight(std::string text, float _scaley) {
    float scaley = (float)SCR_HEIGHT * (float)_scaley;
    float height = 0;
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = Characters[*c];
        float temp = ch.Bearing.y * scaley;
        if (height < temp) { height = temp; }//znajdz najwyzsza litere
    }
    return height;
}
void Text::RenderText(Shader& shader, std::string text, float x, float y, float _scalex, float _scaley, glm::vec3 color) //czy bedzie dobrze dzialac gdy tekst z niknie i bedzie musial sie pojawic nowy?
{
    float scalex = (float)SCR_WIDTH * (float)_scalex;
    float scaley = (float)SCR_HEIGHT * (float)_scaley;
    // activate corresponding render state	
    shader.use();
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scalex;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scaley;

        float w = ch.Size.x * scalex;
        float h = ch.Size.y * scaley;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scalex; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
void Text::ftInit() {
    // FreeType
   // --------
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        //return -1;
    }

    // find path to font
    std::string font_name = "fonts/arial.ttf";
    if (font_name.empty())
    {
        std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
        //return -1;
    }

    // load font as face
    FT_Face face;
    if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        //return -1;
    }
    else {
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++)
        {
            // Load character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            Characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);


    // configure VAO/VBO for texture quads
    // -----------------------------------
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}