#ifndef RT_OBJECTLIST_H
#define RT_OBJECTLIST_H

#include <vector>

#include "RT_Object.h"

/// @brief A collection of RT_Object.
///        Used to group multiple objects and test them together.
class RT_ObjectList : public RT_Object {
    public:
        std::vector<RT_Object*> objects; ///< List of objects.

        /// @brief Default constructor.
        RT_ObjectList() = default;

        /// @brief Creates a list with one object.
        RT_ObjectList(RT_Object* object);

        /// @brief Adds an object to the list.
        void add(RT_Object* object);

        /// @brief Clears all objects.
        void clear();

        /// @brief Checks if list is empty.
        bool empty() const;

        /// @brief Returns number of objects.
        size_t size() const;

        /// @brief Checks intersection with all objects (keeps closest hit).
        bool rayIntersect(const Rayd& ray, const Intervald& t_interval, RT_Record& rec) const override;  

};

#endif