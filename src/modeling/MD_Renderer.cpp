#include "MD_Renderer.h"


MD_Renderer::MD_Renderer(){    
    camera_Main = MD_Camera(0, 3, -3, 0, -2, 2);
    camera_Rt = MD_Camera(0, 1, -3, 0, 0, 2);

    Transform t=Transform::translate(Vec3f(0,0,0));
    MD_Quad q(100,100);
    ground = MD_Object(&q,&t);
}

MD_Renderer::MD_Renderer(const MD_Camera& _camera){    
    camera_Main = _camera;
    camera_Rt = MD_Camera(0, 1, -3, 0, 0, 2);
    
    Transform t=Transform::translate(Vec3f(0,0,0));
    MD_Quad q(100,100);
    ground = MD_Object(&q,&t);
}

MD_Renderer::MD_Renderer(const MD_Camera& _camera_Main,
                                    const MD_Camera& _camera_Rt){    
    camera_Main = _camera_Main;
    camera_Rt = _camera_Rt;

    Transform t=Transform::translate(Vec3f(0,0,0));
    MD_Quad q(100,100);
    ground = MD_Object(&q,&t);
}

void MD_Renderer::addObject(MD_Object* _object){
    objects.push_back(_object);
}

void MD_Renderer::deleteObject(int indice){
    objects.erase(objects.begin() + indice);
}
    
MD_Camera& MD_Renderer::setCameraMain(){
    return camera_Main;
}

MD_Camera& MD_Renderer::setCameraRt(){
    return camera_Rt;
}



void MD_Renderer::render(MD_Shader& shader){
    for(unsigned int i = 0 ; i < objects.size() ; i++) {
        objects.at(i)->draw(shader);
    }
}

