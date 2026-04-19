#version 330 core
out vec4 fragColor;
uniform int uObjectID;
void main() {
    float id = float(uObjectID) / 255.0;
    fragColor = vec4(id, 0.0, 0.0, 1.0);
}