#version 330 core

out vec4 outFragColor;
uniform vec4 outColor;
void main() {
    outFragColor = outColor;
}
