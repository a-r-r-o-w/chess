/**
 * Chess
 * 
 * Written by:
 *    Aryan V S         (PES1UG20CS083)
 *    Aryansh Bhargavan (PES1UG20CS084)
 *    Avanish Bhat      (PES1UG20CS092)
 */

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
