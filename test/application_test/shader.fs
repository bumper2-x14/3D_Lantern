#version 330 core

in vec3 colour 

out frag_Color 

main void{
    frag_Color = colour(0.5,0,0,1);
}
