#version 330 core

#define MAX_LIGHTS 20

out vec4 Color;

in vec3 Norm;
in vec3 FragPos;   // view space
in vec2 Tex;

// lights provided in view space from CPU
uniform vec3  uLightPositions[MAX_LIGHTS];
uniform vec3  uLightColors[MAX_LIGHTS];
uniform int   uNbLights;

uniform float uAmbient;
uniform int   uSpec;

uniform sampler2D texture0;

uniform bool uSelected;

void main() {
    vec3 N = normalize(Norm);
    // in view space the camera is always at the origin
    vec3 V = normalize(-FragPos);

    vec3 result = vec3(0.0);

    for (int i = 0; i < uNbLights; i++) {
        vec3 L = normalize(uLightPositions[i] - FragPos);
        vec3 R = reflect(-L, N);

        vec3 ambient  = uAmbient * uLightColors[i];
        vec3 diffuse  = max(dot(L, N), 0.0) * uLightColors[i];
        vec3 specular = pow(max(dot(V, R), 0.0), float(uSpec)) * uLightColors[i];

        result += ambient + diffuse + specular;
    }

    vec4 texColor = texture(texture0, Tex);
    vec3 color = texColor.rgb * result;

    if (uSelected) {
        color *= vec3(1.3, 1.3, 0.6);
    }

    Color = vec4(color, texColor.a);
}