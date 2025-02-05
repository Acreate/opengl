#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;
out vec2 texCoord;
out vec4 outColor;
void main() {
    gl_Position = vec4(aPos, 1.0f);
    texCoord = aTexCoord;
    outColor = vec4(aColor, 1.0f);;
}
