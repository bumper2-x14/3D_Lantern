#version 330 core
out vec4 fragColor;
uniform int uObjectID;
void main() {
    int objID   = (uObjectID < 10000) ? uObjectID : 0;
    int lightID = (uObjectID >= 10000) ? uObjectID - 10000 : 0;
    fragColor = vec4(float(objID) / 255.0, float(lightID) / 255.0, 0.0, 1.0);
}