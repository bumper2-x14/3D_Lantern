#ifndef MD_SCENE_H
#define MD_SCENE_H

#include <vector>
#include "MD_Object.h"
#include "MD_PointLight.h"
#include "MD_Quad.h"
#include "MD_Sphere.h"
#include "MD_Cylinder.h"
#include "MD_Cone.h"
#include "MD_Torus.h"
#include "MD_Disk.h"

/// @brief Represents the full scene (objects + lights + selection state).
class MD_Scene{
    public:
		std::vector<MD_Object*> objects; ///< List of all objects in the scene.
		std::vector<MD_PointLight*> point_lights; ///< List of all point lights.

		static constexpr int LIGHT_ID_OFFSET = 1000; ///< Offset used to distinguish lights during picking.
		int selected_obj_index = -2; ///< Index of selected object (-2 = none).
		bool show_selected = false; ///< Whether to highlight selected object.
		int selected_light_index = -2; ///< Index of selected light (-2 = none).
    	bool selected_is_light = false; ///< True if current selection is a light.

		/// @brief Constructor (can initialize default scene).
		MD_Scene();

		/// @brief Destructor (cleans all allocated objects/lights).
		~MD_Scene();

		/// @brief Creates and adds a new object to the scene.
		/// @param _name Object name.
		/// @param _shape Shape of the object.
		/// @param _transform Initial transform.
		/// @param _mat Optional material.
		MD_Object* createObject(const std::string& _name, MD_Shape* _shape, const TRSDataf& _transform = {},
								MD_Material* _mat = nullptr);

		/// @brief Removes an object by index.
		void removeObject(int index);

		/// @brief Returns object at given index.
		MD_Object* getObject(int index);

		/// @brief Returns reference to all objects.
		std::vector<MD_Object*>& getObjects();

		/// @brief Selects next object in the list.
		void nextObject();

		/// @brief Creates and adds a point light.
		/// @param position Light position.
		/// @param color Light color.
		/// @param intensity Light intensity.
		MD_PointLight* createPointLight(const Vec3f& position, const Color& color,
										float intensity = 1.0f);

		/// @brief Removes a point light by index.
		void removePointLight(int index);

		/// @brief Returns point light at index.
		MD_PointLight* getPointLight(int i);

		/// @brief Returns reference to all point lights.
		std::vector<MD_PointLight*>& getPointLights();

		/// @brief Loads a default scene with basic objects/lights.
		void loadDefaultScene();

		/// @brief Returns currently selected object (or nullptr).
		MD_Object* getSelectedObject() const;

		/// @brief Returns currently selected light (or nullptr).
		MD_PointLight* getSelectedLight() const;

		/// @brief Clears current selection.
		void deselect();

		/// @brief Selects an object by index.
		void setSelectedObject(int index);

		/// @brief Selects a light by index.
		void setSelectedPointLight(int index);

		// default reusable shapes
		MD_Sphere default_sphere{25, 25}; ///< Default sphere.
		MD_Cylinder default_cylinder{25}; ///< Default cylinder.
		MD_Cone default_cone{25}; ///< Default cone.
		MD_Torus default_torus{25,25}; ///< Default torus.
		MD_Quad default_quad {5, 5}; ///< Default quad.
		MD_Disk default_disk {25}; ///< Default disk.

		// default material
		MD_Material default_material{Vec3f(0.5f, 0.5f, 0.5f), MD_Material::MatType::DIFFUSE};

    private:	
		MD_Quad default_ground{100, 100}; ///< Large quad used as ground.
		MD_Material default_ground_mat{Vec3f(0.5f, 0.5f, 0.5f), MD_Material::MatType::DIFFUSE}; ///< Ground material.
};

#endif