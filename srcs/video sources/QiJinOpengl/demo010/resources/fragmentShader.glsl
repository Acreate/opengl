#version 330 core

out vec4 outFragColor;

in vec2 texCoord;
in vec4 outColor;

uniform sampler2D outTexture;
void main() {
    outFragColor = texture(outTexture, texCoord) * outColor;
}
