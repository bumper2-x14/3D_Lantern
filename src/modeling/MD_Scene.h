#ifndef MD_SCENE_H
#define MD_SCENE_H

#include <vector>
#include "MD_Object.h"
#include "MD_PointLight.h"


class MD_Scene{
    public:
		int selected_index = 0;
		bool show_selected = false;

		MD_Scene();
		~MD_Scene();

		MD_Object* createObject(MD_Shape* _shape, const TRSDataf& _transform = {},
								MD_Material* _mat = nullptr);

		void removeObject(int index);

		MD_Object* getObject(int index);

		std::vector<MD_Object*>& getObjects();

		void nextObject();

		MD_PointLight* createPointLight(const Vec3f& position, const Color& color,
										float intensity = 1.0f);

		void removePointLight(int index);

		MD_PointLight* getPointLight(int i);

		std::vector<MD_PointLight*>& getPointLights();

    private:	
		std::vector<MD_Object*> objects;
		std::vector<MD_PointLight*> point_lights;	
		
};

#endif
