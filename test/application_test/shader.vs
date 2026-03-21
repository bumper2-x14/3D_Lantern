#version 330 core

layout (location=0) in vec3 Pos;
layout (location=1) in vec3 Color;

out vec3 Colour;

uniform mat4 localToWorld;
uniform mat4 worldToView;
uniform mat4 viewToClip;

void main{
    gl_Position = viewToClip * worldToView * localToWorld * vec4(Pos , 1.0); 
    colour=Color;
}
