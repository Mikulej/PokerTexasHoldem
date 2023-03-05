#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"

#include <iostream>
#include <cmath>
#define STBI
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "glm-master/glm/glm.hpp"
#include "glm-master/glm/gtc/matrix_transform.hpp"
#include "glm-master/glm/gtc/type_ptr.hpp"

#include <filesystem>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <map>
#include "Image.h"
#include "StaticObject.h"
#include "Text.h"
#include "PokerEngine.h"

#include <ft2build.h>
#include FT_FREETYPE_H

inline bool mouse_click();
inline void options_handler();
inline void main_menu_handler();
inline void options_init();
inline void exit_program_init();
inline void main_menu_init();
inline void play_init();
