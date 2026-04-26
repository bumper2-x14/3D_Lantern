#ifndef MD_OBJECT_H
#define MD_OBJECT_H

#include "modeling/MD_Shape.h"
#include "modeling/MD_Shader.h"
#include "modeling/MD_Material.h"
#include "math/TRS_data.h"

/// @brief Represents an object in the scene.
///        Combines a shape (geometry), a material (appearance) and a transform.
class MD_Object {
    public:
        TRSDataf trs; ///< Translation, Rotation, Scale of the object.
        bool selectable = true; ///< Indicates if the object can be selected (e.g. in UI).
        std::string name; ///< Name of the object.

        /// @brief Creates an object with optional shape, material and transform.
        /// @param _name Name of the object.
        /// @param shape Pointer to the shape (geometry).
        /// @param material Pointer to the material (appearance).
        /// @param _trs_data Initial transform data.
        MD_Object(const std::string& _name, MD_Shape* shape = nullptr,
                        MD_Material* material = nullptr,
                                const TRSDataf& _trs_data = {});

        /// @brief Returns the shape of the object.
        MD_Shape* getShape()const { return shape; }
        
        /// @brief Returns the material of the object.
        MD_Material* getMaterial()const { return material; }

        /// @brief Sets the material of the object.
        void setMaterial(MD_Material* m) { material = m; }

        /// @brief Computes and returns the transformation matrix from TRS data.
        Mat4f getTransformMatrix() const;

        /// @brief Draws the object using its shape and material.
        /// @param shader Shader used for rendering.
        void draw(MD_Shader& shader);

        /// @brief Serializes the object into a string (for saving/exporting).
        std::string serialize() const;
        
    private:
        MD_Shape* shape; ///< Pointer to the geometry.
        MD_Material* material; ///< Pointer to the material.
};

#endif