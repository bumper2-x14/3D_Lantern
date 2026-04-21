#ifndef MD_SCENE_H
#define MD_SCENE_H

#include <vector>
#include "MD_Object.h"
#include "MD_PointLight.h"
#include "MD_Quad.h"
#include "MD_Sphere.h"


class MD_Scene{
    public:
		std::vector<MD_Object*> objects;
		std::vector<MD_PointLight*> point_lights;

		static constexpr int LIGHT_ID_OFFSET = 1000;
		int selected_obj_index = -2;
		bool show_selected = false;
		int selected_light_index = -2;
    	bool selected_is_light = false;

		MD_Scene();
		~MD_Scene();

		MD_Object* createObject(const std::string& _name, MD_Shape* _shape, const TRSDataf& _transform = {},
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

		void loadDefaultScene();

		MD_Object* getSelectedObject() const;
		MD_PointLight* getSelectedLight() const;
		void deselect();

		void setSelectedObject(int index);
		void setSelectedPointLight(int index);

    private:	
		MD_Quad default_ground{100, 100};
		MD_Sphere default_sphere{25, 25};

		MD_Material default_ground_mat{Vec3f(0.5f, 0.5f, 0.5f), MD_Material::MatType::DIFFUSE};
		MD_Material default_sphere_mat{Vec3f(0.2f, 0.6f, 1.0f), MD_Material::MatType::DIFFUSE};
		
};

#endif
