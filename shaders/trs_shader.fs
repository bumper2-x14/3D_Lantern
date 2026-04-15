#version 330 core
#define MAX_LIGHTS 20

in vec2 v_uv;
in vec3 v_normal;
in vec3 v_fragPos;

out vec4 fragColor;

// ===== MATERIAL =====
uniform int       u_materialType;   // 0=texture,1=ambient,2=diffuse,3=specular
uniform sampler2D u_texture;
uniform vec3      u_color;
uniform float     u_shininess;

// ===== LIGHTS =====
uniform vec3  uLightPositions[MAX_LIGHTS];
uniform vec3  uLightColors[MAX_LIGHTS];
uniform float uLightIntensities[MAX_LIGHTS];
uniform int   uNbLights;

// ===== CAMERA =====
uniform vec3 u_viewPos;

// ===== GLOBAL =====
uniform float uAmbient;

void main() {

    // ===== BASE COLOR =====
    vec3 baseColor = (u_materialType == 0)
        ? texture(u_texture, v_uv).rgb
        : u_color;

    // ===== AMBIENT ONLY (NO LIGHTING) =====
    if (u_materialType == 1) {
        fragColor = vec4(baseColor * uAmbient, 1.0);
        return;
    }

    // ===== COMMON VECTORS =====
    vec3 N = normalize(v_normal);
    vec3 V = normalize(u_viewPos - v_fragPos);

    vec3 result = vec3(0.0);

    // ===== MULTI-LIGHT LOOP =====
    for (int i = 0; i < uNbLights; i++) {

        vec3 L = normalize(uLightPositions[i] - v_fragPos);
        float dist  = length(uLightPositions[i] - v_fragPos);

        // attenuation
        float atten = 1.0 / (1.0 + 0.09 * dist + 0.032 * dist * dist);
        float inten = uLightIntensities[i] * atten;

        // ===== DIFFUSE =====
        float diff = max(dot(N, L), 0.0);
        vec3 diffuse = diff * uLightColors[i] * inten;

        vec3 specular = vec3(0.0);

        // ===== SPECULAR ONLY IF ENABLED =====
        if (u_materialType == 3) {
            vec3 R = reflect(-L, N);
            float spec = pow(max(dot(V, R), 0.0), u_shininess);
            specular = spec * uLightColors[i] * inten;
        }

        result += diffuse + specular;
    }

    // ===== FINAL COLOR =====
    vec3 lighting = result;

    if (uNbLights == 0) {
        lighting = vec3(1.0); // fallback white light
    }

    lighting += uAmbient;
    vec3 finalColor = baseColor * lighting;
    fragColor = vec4(finalColor, 1.0);
}