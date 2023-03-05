#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
out vec2 TexCoord;
uniform mat4 transform;//wszystkie przeksztalcenia powinny byc na transform
uniform vec2 texOffset;
uniform vec2 texZoom;
void main()
{
gl_Position = transform * vec4(aPos, 1.0f);
TexCoord = vec2((aTexCoord.x + texOffset.x) / texZoom.x, (aTexCoord.y+ texOffset.y) / texZoom.y);
}
