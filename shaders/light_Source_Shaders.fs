#version 330 core

out vec3 fragmentColor;

uniform vec3 lightColor;

void main(){
    fragmentColor = vec4(lightColor, 1.0f);
}

