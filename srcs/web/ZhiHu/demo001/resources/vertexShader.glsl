#version 330 core
layout(location = 0) in vec3 point;
layout(location = 1) in vec3 point2;
layout(location = 2) in vec3 point3;

uniform uvec3 vk1;
uniform uvec3 vk2;



out vec4 color1;
out vec4 color2;
uvec4 bbd;
void main() {
    gl_Position = vec4(point, 1.0f);
    color2 = uvec4(point3, 1.0f);
    color1 = uvec4(vk2, 1.0f);
}
