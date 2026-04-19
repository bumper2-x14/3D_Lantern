#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec3 v_normal;
out vec3 v_fragPos;
out vec2 v_uv;

uniform mat4 uProjection;
uniform mat4 uLocalToView;

void main() {
    vec4 viewPos = uLocalToView * vec4(aPos, 1.0);
    gl_Position  = uProjection * viewPos;
    v_fragPos    = vec3(viewPos);

    mat3 normalMat = transpose(inverse(mat3(uLocalToView)));
    v_normal       = normalMat * aNormal;
    v_uv           = aTexCoord;
}