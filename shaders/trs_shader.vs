#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 Norm;
out vec3 FragPos;
out vec2 Tex;

uniform vec3 uTranslation;
uniform vec3 uRotation;   // euler angles, degrees
uniform vec3 uScale;

uniform mat4 worldToView;
uniform mat4 viewToClip;

mat4 buildTRS() {
    vec3 r = radians(uRotation);

    float cx = cos(r.x), sx = sin(r.x);
    float cy = cos(r.y), sy = sin(r.y);
    float cz = cos(r.z), sz = sin(r.z);

    mat4 S = mat4(
        uScale.x, 0,        0,        0,
        0,        uScale.y, 0,        0,
        0,        0,        uScale.z, 0,
        0,        0,        0,        1
    );
    mat4 Rx = mat4(
        1,  0,   0,  0,
        0,  cx, -sx, 0,
        0,  sx,  cx, 0,
        0,  0,   0,  1
    );
    mat4 Ry = mat4(
         cy, 0, sy, 0,
         0,  1,  0, 0,
        -sy, 0, cy, 0,
         0,  0,  0, 1
    );
    mat4 Rz = mat4(
        cz, -sz, 0, 0,
        sz,  cz, 0, 0,
        0,   0,  1, 0,
        0,   0,  0, 1
    );
    mat4 T = mat4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        uTranslation.x, uTranslation.y, uTranslation.z, 1
    );

    return T * Rz * Ry * Rx * S;
}

void main() {
    mat4 localToWorld = buildTRS();
    mat4 localToView  = worldToView * localToWorld;

    gl_Position = viewToClip * localToView * vec4(aPos, 1.0);

    // Normal matrix: upper-left 3x3 of transpose(inverse(localToWorld))
    // Computed in view space so Norm and FragPos are in the same space
    Norm    = mat3(transpose(inverse(localToView))) * aNormal;
    FragPos = vec3(localToView * vec4(aPos, 1.0));
    Tex     = aTexCoord;
}