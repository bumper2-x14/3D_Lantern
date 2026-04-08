#version 330 core

layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoord;

out vec3 Norm;
out vec3 Frag_Pos;
out vec2 Tex;

uniform mat4 localToWorld;
uniform mat4 worldToView;
uniform mat4 viewToClip;
    

void main(){
    gl_Position = viewToClip * worldToView * localToWorld * vec4(Pos, 1.0);
    Norm = mat3(transpose(inverse(localToWorld))) * Normal; 
    Frag_Pos = vec3(worldToView * vec4(Pos , 1.0)) ;
    Tex = TexCoord;

}
