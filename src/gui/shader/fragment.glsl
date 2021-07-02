#version 330 core

in vec4 Color;
in vec2 TextureCoordinates;

uniform bool u_isTexture = false;
uniform sampler2D u_Texture;

out vec4 fragColor;

void main () {
    if (u_isTexture)
        fragColor = texture(u_Texture, TextureCoordinates);
    else
        fragColor = Color;
}
