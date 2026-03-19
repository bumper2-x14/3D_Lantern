#include "MD_Shader.h"

MD_Shader::MD_Shader(const std::string &vertex_path, const std::string &fragment_path){
    //declering variables
    const char* vertex_source;
    const char* fragment_source;
    //strings
    std::string vertex_code_strg;
    std::string fragment_code_strg;
    //string streams
    std::stringstream vertex_code_strm;
    std::stringstream fragment_code_strm;
    //ifstreams to open our files
    std::ifstream vertex_file;
    std::ifstream fragment_file;
    //opening our file
    vertex_file.open(vertex_path);
    fragment_file.open(fragment_path);
    //putting all of our files contents into string streams
    if(vertex_file.is_open() && fragment_file.is_open()){
        vertex_code_strm<<vertex_file.rdbuf();
        fragment_code_strm<<fragment_file.rdbuf();
    }
    else{
        std::cout<<"could not open the file"<<std::endl;
        std::exit(EXIT_FAILURE);
    }

    vertex_file.close();
    fragment_file.close();
    //converting string streams into strings 
    vertex_code_strg=vertex_code_strm.str();
    fragment_code_strg=fragment_code_strm.str();
    //converting string into chars
    vertex_source=vertex_code_strg.c_str();
    fragment_source=fragment_code_strg.c_str();
    
    //declering variables for our object IDs
    unsigned int vertex_shader_object,fragment_shader_object;
    //creating our objects
    vertex_shader_object = glCreateShader( GL_VERTEX_SHADER);
    fragment_shader_object = glCreateShader( GL_FRAGMENT_SHADER);
    //putting the source into the shader objects
    glShaderSource(vertex_shader_object, 1, &vertex_source, NULL);
    glShaderSource(fragment_shader_object, 1, &fragment_source, NULL);

    
    int success;
    char log[512];
    //compiling the object
    glCompileShader(vertex_shader_object);
    //testing for errors
    glGetShaderiv(vertex_shader_object ,GL_COMPILE_STATUS ,&success);
    if(!success){
        glGetShaderInfoLog(vertex_shader_object ,512 , NULL, log);
        std::cout<< "vertex shader error " << log << std::endl; 
    }
    //compiling the object
    glCompileShader(fragment_shader_object);
    //testing for errors
    glGetShaderiv(fragment_shader_object ,GL_COMPILE_STATUS ,&success);
    if(!success){
        glGetShaderInfoLog(vertex_shader_object ,512 , NULL, log);
        std::cout<< "fragment shader error " << log << std::endl; 
    }

    
    //creating a shader program
    program_id=glCreateProgram(); 
    //attaching our objects to the program
    glAttachShader(program_id ,vertex_shader_object);
    glAttachShader(program_id ,fragment_shader_object);
    //linking the objects
    glLinkProgram(program_id);
    //testing for errors
    glGetProgramiv(program_id ,GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(program_id ,512 , NULL , log);
        std::cout<<"linking error " << log << std::endl;
    }

    //deleting the shader objects
    glDeleteShader(vertex_shader_object);
    glDeleteShader(fragment_shader_object);

}

void MD_Shader::apply(){
    glUseProgram(program_id);
}

void MD_Shader::setFloat(const std::string &name ,float value)const{
    int location = glGetUniformLocation(program_id ,name.c_str());
    if(location == -1)
        std::cout<<"could not find uniform location"<<std::endl;
    else
        glUniform1f(location,value);
}

void MD_Shader::setInt(const std::string &name ,int value)const{
    int location = glGetUniformLocation(program_id ,name.c_str());
    if(location == -1)
        std::cout<<"could not find uniform location"<<std::endl;
    else
        glUniform1i(location ,value);
}

void MD_Shader::setBool(const std::string &name ,bool value)const{
    int location = glGetUniformLocation(program_id ,name.c_str());
    if(location == -1)
        std::cout<<"could not find uniform location"<<std::endl;
    else
        glUniform1i(location ,(int)value);
}

void MD_Shader::setVec2(const std::string &name ,const Vec2f &value){
    int location = glGetUniformLocation(program_id ,name.c_str());
    if(location == -1)
        std::cout<<"could not find uniform location"<<std::endl;
    else
        glUniform2f(location ,value.x , value.y);
}

void MD_Shader::setVec3(const std::string &name ,const Vec3f &value){
    int location = glGetUniformLocation(program_id ,name.c_str());
    if(location == -1)
        std::cout<<"could not find uniform location"<<std::endl;
    else
        glUniform3f(location ,value.x ,value.y ,value.z);
}

void MD_Shader::setMat4(const std::string &name ,const Mat4f &value){
    int location = glGetUniformLocation(program_id ,name.c_str());
    if(location == -1)
        std::cout<<"could not find uniform location"<<std::endl;
    else
        glUniformMatrix4fv(location ,1 ,GL_FALSE ,value.data());
}

unsigned int MD_Shader::get_program_id()const{
    return program_id;
}

void MD_Shader::regression_test(){
    MD_Shader shader("regress_test.vs" , "regress_test.fs" );
    assert(shader.program_id>0);
}
