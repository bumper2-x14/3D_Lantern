#version 330 core 
#define MAX_LIGHT 20 
out vec4 Color; 

in vec3 Norm;
in vec3 Frag_Pos;
in vec2 Tex;

uniform vec3 light_Colors[MAX_LIGHT];
uniform int nb_Light_Color;

uniform vec3 light_Positions[MAX_LIGHT];
uniform int nb_Light_Pos;

uniform vec3 view_Pos;

uniform float ambient;
uniform int spec;

uniform sampler2D texture0;


//to be added more texture surfaces

void main(){
    vec3 result = vec3(0.0);
        for(int i=0; i<nb_Light_Pos; i++){
        
            vec3 r_Ambient = ambient * light_Colors[i] ;
            vec3 Normal = normalize(Norm);
            vec3 light_Dir = normalize(light_Positions[i] - Frag_Pos); 
            vec3 r_Diffuse = light_Colors[i] * (max(dot(light_Dir, Normal), 0.0));
    
            vec3 view_Dir = normalize(view_Pos - Frag_Pos);
            vec3 reflect_Dir = reflect(-light_Dir, Normal);
            vec3 r_Spec = pow(max(dot(view_Dir, reflect_Dir), 0.0),
                                            spec) * light_Colors[i];

            result = r_Spec + r_Diffuse + r_Ambient + result ; 
        
        }
    
    //result = clamp(result, 0.0, 1.0); 
    //light result is not affected for testing perpuses
    //Color = vec4(clamp(result, 0.0, 1.0), 1.0);

    vec4 texColor = texture(texture0, Tex);
    
    Color = vec4(texColor.rgb * result , texColor.a); 
    //Color = texColor ;
}
