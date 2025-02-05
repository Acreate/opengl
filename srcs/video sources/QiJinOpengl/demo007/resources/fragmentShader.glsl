#version 330 core

out vec4 outFragColor;
in vec4 outColor;
void main() {
    outFragColor = outColor;
}
