#version 330 core

out vec3 v_nearPoint;
out vec3 v_farPoint;

uniform mat4 uProjection;
uniform mat4 uView;

vec2 positions[4] = vec2[](
    vec2(-1.0, -1.0),
    vec2( 1.0, -1.0),
    vec2(-1.0,  1.0),
    vec2( 1.0,  1.0)
);

vec3 unproject(vec2 p, float z) {
    mat4 invVP = inverse(uProjection * uView);
    vec4 world = invVP * vec4(p, z, 1.0);
    return world.xyz / world.w;
}

void main() {
    vec2 p      = positions[gl_VertexID];
    v_nearPoint = unproject(p, -1.0);
    v_farPoint  = unproject(p,  1.0);
    gl_Position = vec4(p, 0.0, 1.0);
}
