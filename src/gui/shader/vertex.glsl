#version 330 core

layout(location = 0) in vec2 VertexPosition;
layout(location = 1) in vec2 TexturePosition;

uniform float u_RowShift = 0.0f;
uniform float u_ColShift = 0.0f;
uniform vec4  u_Color;

out vec4 Color;
out vec2 TextureCoordinates;

void main () {
    gl_Position = vec4(VertexPosition.x + u_RowShift, VertexPosition.y - u_ColShift, 0.0f, 1.0f);
    Color = u_Color;
    TextureCoordinates = TexturePosition;
}
