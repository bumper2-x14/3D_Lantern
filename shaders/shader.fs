#version 330 core

uniform bool uWireframe;
out vec4 frag_Color; 

void main(){
    frag_Color = uWireframe ? vec4(0.0, 0.0, 0.0, 1.0) : vec4(0.5, 0.5, 0.5, 1.0);
}
