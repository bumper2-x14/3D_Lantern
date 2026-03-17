#ifndef _CAMERA_ 
#define _CAMERA_

#include <math/vec3.h> 
#include <math/mat4.h>
#include <math/utility.h>

class MD_Camera {
        public:
        //constructors
        MD_Camera();
        MD_Camera( const Vec3f& _cameraPos);   
        MD_Camera( const Vec3f& _cameraPos, const Vec3f& _cameraFront);
        MD_Camera( float x, float y, float z, const Vec3f& _cameraFront);
        MD_Camera( float x, float y, float z, float dx, float dy, float dz);
        MD_Camera( const Vec3f& _cameraPos, float dx, float dy, float dz);


        Vec3f getCameraPos();
        Vec3f getCameraFront();
        Vec3f getCameraUp();
        float getPitch();
        float getYaw();
        unsigned int getIdShader(); 


        float getSensitivity()const;
        void setSensitivity( float sensitivity);
        
        //to generate the lookAt matrices
        Mat4f genLookAt()const;
                            

        //put the shader id in our camera class
        void setShader(int _idShader);

        //change our camera object variable so that it moves around and looks around
        void update( bool w ,bool s ,bool a ,bool d ,
                        float xoffset ,float yoffset ,float speed );
        
        //generates and puts the look at matrice in shader uniform
        void setView()const;

        static void regressionTest();
    
    private:
        Vec3f cameraPos;
        Vec3f cameraFront;
        Vec3f cameraUp;
        float yaw=-90;
        float pitch=0;
        float sensitivity=0.50;
        unsigned int idShader;
};
#endif
