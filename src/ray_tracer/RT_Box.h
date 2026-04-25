#ifndef RT_BOX_H
#define RT_BOX_H
#include "RT_Object.h"
#include "RT_Quad.h"
#include "RT_Material.h"

/**
 * @file RT_Box.h
 * @brief Declaration of the RT_Box class representing a 3D axis-aligned box.
 */

/**
 * @class RT_Box
 * @brief Represents a rectangular 3D box composed of six planar faces.
 *
 * This class inherits from RT_Object and models an axis-aligned box
 * defined by two opposite corners.
 *
 * The box is built using six RT_Quad objects, each representing one face.
 */
class RT_Box : public RT_Object {
public:
    /**
     * @brief Constructs a box with the given material.
     *
     * @param _material Pointer to the material applied to the box.
     */
    // Axis-aligned box defined by two opposite corners
    RT_Box(RT_Material* _material);

    /**
     * @brief Applies a geometric transformation to the box.
     *
     * Updates the position, rotation, and/or scale of the box
     * using a TRS transformation.
     *
     * @param _transfom Transformation to apply.
     */
    void setTransform(const TRSTransformd& _transfom) override;

    /**
     * @brief Tests the intersection between a ray and the box.
     *
     * Checks whether the ray intersects any face of the box
     * within the given interval.
     *
     * @param ray Incoming ray.
     * @param t_interval Valid ray distance interval.
     * @param rec Structure receiving intersection data.
     *
     * @return true if an intersection is found.
     * @return false otherwise.
     */
    bool rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const override;

    static void regressionTest();

private:
    RT_Quad faces[6]; ///< Array of the six faces of the box.
    RT_Material* material; ///< Pointer to the material applied to the box.
};

#endif