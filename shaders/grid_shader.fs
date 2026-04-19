#version 330 core

in vec3 v_nearPoint;
in vec3 v_farPoint;

out vec4 fragColor;

uniform mat4 uProjection;
uniform mat4 uView;

void main() {
    float denom = v_farPoint.y - v_nearPoint.y;

    // discard if ray is parallel to ground or pointing away
    if (abs(denom) < 1e-5) discard;

    float t = -v_nearPoint.y / denom;
    if (t < 0.0 || t > 1.0) discard;   // only between near and far plane

    vec3 pos = v_nearPoint + t * (v_farPoint - v_nearPoint);

    // write depth
    vec4 clip = uProjection * uView * vec4(pos, 1.0);
    gl_FragDepth = (clip.z / clip.w) * 0.5 + 0.5;

    // grid lines
    vec2 coord = pos.xz;
    vec2 grid  = abs(fract(coord - 0.5) - 0.5) / fwidth(coord);
    float line = min(grid.x, grid.y);
    float alpha = 1.0 - clamp(line, 0.0, 1.0);

    // fade with distance
    float fade = 1.0 - clamp(length(pos.xz) / 30.0, 0.0, 1.0);
    alpha *= fade;

    if (alpha < 0.01) discard;

    // axis colors
    vec3 color = vec3(0.5, 0.5, 0.5);
    if (abs(pos.z) < fwidth(pos.z) * 1.5) color = vec3(1.0, 0.3, 0.3); // X axis
    if (abs(pos.x) < fwidth(pos.x) * 1.5) color = vec3(0.3, 0.3, 1.0); // Z axis

    fragColor = vec4(color, alpha * 0.8);
}