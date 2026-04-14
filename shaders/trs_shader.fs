#version 330 core
#define MAX_LIGHTS 20

in vec3 Norm;
in vec3 FragPos;
in vec2 Tex;

out vec4 Color;

uniform vec3  uLightPositions[MAX_LIGHTS];   // in view space
uniform vec3  uLightColors[MAX_LIGHTS];
uniform float uLightIntensities[MAX_LIGHTS];
uniform int   uNbLights;

uniform float uAmbient;
uniform int   uSpec;
uniform vec3  uSelectionTint;
uniform bool  uSelected;

uniform sampler2D texture0;

void main() {
    vec3 N = normalize(Norm);
    vec3 V = normalize(-FragPos);  // camera at origin in view space
    vec3 result = vec3(0.0);

    for (int i = 0; i < uNbLights; i++) {
        vec3  L     = normalize(uLightPositions[i] - FragPos);
        vec3  R     = reflect(-L, N);
        float dist  = length(uLightPositions[i] - FragPos);
        float atten = 1.0 / (1.0 + 0.09 * dist + 0.032 * dist * dist);
        float inten = uLightIntensities[i] * atten;

        vec3 ambient  = uAmbient * uLightColors[i];
        vec3 diffuse  = max(dot(L, N), 0.0)                    * uLightColors[i] * inten;
        vec3 specular = pow(max(dot(V, R), 0.0), float(uSpec)) * uLightColors[i] * inten;

        result += ambient + diffuse + specular;
    }

    vec4 texColor = texture(texture0, Tex);
    vec3 color    = texColor.rgb * result;

    if (uSelected)
        color *= uSelectionTint;

    Color = vec4(color, texColor.a);
}