#ifndef _CAMERA_ 
#define _CAMERA_

#include <math/vec3.h> 
#include <math/mat4.h>
#include <math/utility.h>

class MD_Camera {
        public:
        MD_Camera();
        MD_Camera(Vec3f _cameraPos);   
        MD_Camera(Vec3f _cameraPos,Vec3f _cameraFront);
        MD_Camera(float x, float y,float z,Vec3f _cameraFront);
        MD_Camera(float x,float y,float z,float dx,float dy,float dz);
        MD_Camera(Vec3f _cameraPos,float dx,float dy,float dz);

        Vec3f getCameraPos();
        Vec3f getCameraFront();
        Vec3f getCameraUp();
        float getPitch();
        float getYaw();
        unsigned int getIdShader(); 

        float getSensevity();

        void setSensevity(float sensevity);

        Mat4f genLookAt();
    
        void setShader(int _idShader);

        void update(bool w,bool s,bool a,bool d,
                        float xoffset,float yoffset,float speed);
        void setView();

        static void regressionTest();
    
    private:
        Vec3f cameraPos;
        Vec3f cameraFront;
        Vec3f cameraUp;
        float yaw=0;
        float pitch=0;
        float sensevity=0.50;
        unsigned int idShader;
};
#endif
