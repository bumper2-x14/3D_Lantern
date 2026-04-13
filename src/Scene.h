#include <vector.h>
#include "MD_Object.h"
#include "MD_Camera.h"
#include "MD_PointLight"


struct Setting {
	//to be complited
};


class Scene{
    public:
	Scene();
	
	void addObject(const &object ob);
	void addObject(MD_Shape* shape = nullptr,
				Transform* transform = nullptr, 
					MD_Material* _material = nullptr);
    	
	void setCamera(const &Vec3f camera_Pos = Vec3f(0, 3, 0),
				const &Vec3f camera_Direction = Vec3f(0, -1, 0));
	
	//void updateCamere();
    
	const MD_Object& getObject(int i);
	const MD_Camera& getCamera(); 

    private:
	
	vectore<MD_Object*> objects;
	MD_Camera main_Camera;
	vectore<MD_PointLight*> point_Lights;
	Setting setting;	
	
};
