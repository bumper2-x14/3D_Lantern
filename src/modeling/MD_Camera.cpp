#include "MD_Camera.h"
#include <cmath>
#include <glad/glad.h>
#include <iostream>

//constructors
MD_Camera::MD_Camera(){
    cameraPos=Vec3f(0.0,0.0,0.0);
    cameraFront=Vec3f( 0.0, 0.0, -1.0);
    cameraUp=Vec3f(0.0,1.0,0.0);
}
MD_Camera::MD_Camera(const Vec3f& _cameraPos){
    cameraPos=_cameraPos;
    cameraFront=Vec3f( 0.0, 0.0, -1.0);
    cameraUp=Vec3f( 0.0, 1.0, 0.0);
}
MD_Camera::MD_Camera( const Vec3f& _cameraPos, const Vec3f& _cameraFront){
    cameraPos=_cameraPos;
    cameraFront=_cameraFront;
    cameraUp=Vec3f( 0.0, 1.0, 0.0);
}
MD_Camera::MD_Camera( float x, float y, float z, const Vec3f& _cameraFront){
    cameraPos=Vec3f( x, y, z);
    cameraFront=_cameraFront;
    cameraUp=Vec3f( 0.0, 1.0, 0.0);
}
MD_Camera::MD_Camera( float x, float y, float z,
                            float dx, float dy, float dz){
    cameraPos=Vec3f( x, y, z);
    cameraFront=Vec3f( dx, dy, dz);
    cameraUp=Vec3f( 0.0, 1.0, 0.0);
}
MD_Camera::MD_Camera(const Vec3f& _cameraPos, float dx, float dy, float dz){
    cameraPos=_cameraPos;
    cameraFront=Vec3f( dx, dy, dz);
    cameraUp=Vec3f( 0.0, 1.0, 0.0);
}


float MD_Camera::getSensitivity()const{
    return sensitivity;
}
void MD_Camera::setSensitivity(float _sensitivity){
    sensitivity = _sensitivity ;
}

//to generate the lookAt matrices
Mat4f MD_Camera::genLookAt()const{
    return Mat4f::lookAt( cameraPos,
                 cameraFront+cameraPos,
                 cameraUp);
}

//put the shader id in our camera class
void MD_Camera::setShader(int _idShader){
    idShader=_idShader;
}

//change our camera object variable so that it moves around and looks around
void MD_Camera::update( bool w, bool s, bool a, bool d,
        float xoffset,float yoffset,float speed){
    //to move around
    if(w){
        cameraPos+=speed*cameraFront;
    }
    if(s){
        cameraPos-=speed*cameraFront;
    }
    if(d){
        cameraPos+=speed*normalize( cross( cameraFront, cameraUp));
    }
    if(a){
        cameraPos-=speed*normalize( cross( cameraFront, cameraUp));
    }

    //to look around 
    Vec3f direction=cameraFront;
    yaw+= xoffset * sensitivity;
    pitch+= yoffset * sensitivity;
    direction.x = std::cos( toRadians( yaw)) * std::cos( toRadians( pitch));
    direction.y = std::sin( toRadians( pitch));
    direction.z = std::sin( toRadians( yaw)) * std::cos( toRadians( pitch));

    cameraFront=normalize(direction);
}

//generates and puts the look at matrice in shader uniform
void MD_Camera::setView()const{
    Mat4f viewMatrix= genLookAt();
    int viewUniform= glGetUniformLocation( idShader, "worldToView");
    if(viewUniform == -1)
        std::cout<<"uniform not found"<<std::endl;
    glUniformMatrix4fv( viewUniform, 1, GL_FALSE, viewMatrix.data());
}


void MD_Camera::regressionTest(){
    MD_Camera testCamera;
    testCamera.update( 1, 0, 0, 0, 0, 0, 30);
    assert(testCamera.cameraPos.z <0.0);
}

