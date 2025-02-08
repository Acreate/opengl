#version 330 core

uniform uvec3 fk1;
uniform uvec3 fk2;
uniform uvec3 fk3;


out vec4 color;
in vec4 color2;
in vec4 color1;

uvec3 bbd;
void main() {
    color = color2 * color1;
}
