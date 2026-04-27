#ifndef KEYWORD_MAP_H
#define KEYWORD_MAP_H

#include <unordered_map>
#include <string>

/// @brief Main block types that can appear in a scene file.
enum class BlockType {
    SETTING, CAMERA, MATERIAL, TEXTURE, OBJECT, LIGHT, UNKNOWN
};

/// @brief Object/shape types supported by the parser.
enum class ObjectType {
    SPHERE, CYLINDER, CONE, BOX, QUAD, DISK, TORUS, MESH, UNKNOWN
};

/// @brief Texture types supported in the scene description.
enum class TextureType {
    SOLIDCOLOR, IMAGE, CHECKER, NOISE, TURBULENCE, MARBLE, WOOD, WARPED, UNKNOWN
};

/// @brief Material types supported by the ray tracer.
enum class MaterialType {
    LAMBERTIAN, METALLIC, DIELECTRIC, ISOTROPIC, EMISSIVE, UNKNOWN
};

/// @brief Light types supported by the scene.
enum class LightType {
    POINT, DIRECTIONAL, UNKNOWN
};

/// @brief Field names used inside scene blocks.
enum class IdentType {
    TYPE,
    NAME,
    FILE,
    WIDTH,
    HEIGHT,
    SAMPLES,
    DEPTH,
    ASPECT_RATIO,
    BACKGROUND,
    POSITION,
    LOOKAT,
    UP_VIEW,
    VFOV,
    DEFOCUS_ANGLE,
    FOCUS_DISTANCE,
    COLOR,
    EVEN,
    ODD,
    SCALE,
    TEXTURE_REF,
    FUZZ,
    IOR,
    INTENSITY,
    SHAPE,
    MATERIAL_REF,
    RADIUS,
    Y_MIN,
    Y_MAX,
    CAPPED,
    MIN,
    MAX,
    NORMAL,
    MAJOR_RADIUS,
    MINOR_RADIUS,
    TRANSLATE,
    ROTATE,
    DIRECTION,
    UNKNOWN
};

/// @brief Converts a string keyword into a BlockType.
inline BlockType convertToBlockType(const std::string& s) {
    static const std::unordered_map<std::string, BlockType> m {
        { "setting",  BlockType::SETTING  },
        { "camera",   BlockType::CAMERA   },
        { "material", BlockType::MATERIAL },
        { "texture",  BlockType::TEXTURE  },
        { "object",   BlockType::OBJECT   },
        { "light",    BlockType::LIGHT    }
    };
    auto it = m.find(s);
    return it != m.end() ? it->second : BlockType::UNKNOWN;
}

/// @brief Converts a string keyword into an ObjectType.
inline ObjectType convertToObjectType(const std::string& s) {
    static const std::unordered_map<std::string, ObjectType> m {
        { "sphere",   ObjectType::SPHERE   },
        { "cylinder", ObjectType::CYLINDER },
        { "cone",     ObjectType::CONE     },
        { "box",      ObjectType::BOX      },
        { "quad",     ObjectType::QUAD     },
        { "disk",     ObjectType::DISK     },
        { "torus",     ObjectType::TORUS   },
        { "mesh",     ObjectType::MESH     }
    };
    auto it = m.find(s);
    return it != m.end() ? it->second : ObjectType::UNKNOWN;
}

/// @brief Converts a string keyword into a TextureType.
inline TextureType convertToTextureType(const std::string& s) {
    static const std::unordered_map<std::string, TextureType> m {
        { "solidcolor", TextureType::SOLIDCOLOR },
        { "image",      TextureType::IMAGE      },
        { "checker",    TextureType::CHECKER    },
        { "noise",      TextureType::NOISE      },
        { "turbulence", TextureType::TURBULENCE },
        { "marble",     TextureType::MARBLE     },
        { "wood",       TextureType::WOOD       },
        { "warped",     TextureType::WARPED     }
    };
    auto it = m.find(s);
    return it != m.end() ? it->second : TextureType::UNKNOWN;
}

/// @brief Converts a string keyword into a MaterialType.
inline MaterialType convertToMaterialType(const std::string& s) {
    static const std::unordered_map<std::string, MaterialType> m {
        { "lambertian", MaterialType::LAMBERTIAN },
        { "metallic",   MaterialType::METALLIC   },
        { "dielectric", MaterialType::DIELECTRIC },
        { "isotropic",  MaterialType::ISOTROPIC  },
        { "emissive",   MaterialType::EMISSIVE   }
    };
    auto it = m.find(s);
    return it != m.end() ? it->second : MaterialType::UNKNOWN;
}

/// @brief Converts a string keyword into a LightType.
inline LightType convertToLightType(const std::string& s) {
    static const std::unordered_map<std::string, LightType> m {
        { "point",       LightType::POINT       },
        { "directional", LightType::DIRECTIONAL }
    };
    auto it = m.find(s);
    return it != m.end() ? it->second : LightType::UNKNOWN;
}

/// @brief Converts a string keyword into an IdentType.
inline IdentType convertToIdentType(const std::string& s) {
    static const std::unordered_map<std::string, IdentType> m {
        { "type",           IdentType::TYPE             },
        { "name",           IdentType::NAME             },
        { "file",           IdentType::FILE             },
        { "width",          IdentType::WIDTH            },
        { "height",         IdentType::HEIGHT           },
        { "samples",        IdentType::SAMPLES          },
        { "depth",          IdentType::DEPTH            },
        { "aspect_ratio",   IdentType::ASPECT_RATIO     },
        { "background",     IdentType::BACKGROUND       },
        { "position",       IdentType::POSITION         },
        { "lookat",         IdentType::LOOKAT           },
        { "up_view",        IdentType::UP_VIEW          },
        { "vfov",           IdentType::VFOV             },
        { "defocus_angle",  IdentType::DEFOCUS_ANGLE    },
        { "focus_distance", IdentType::FOCUS_DISTANCE   },
        { "color",          IdentType::COLOR            },
        { "even",           IdentType::EVEN             },
        { "odd",            IdentType::ODD              },
        { "scale",          IdentType::SCALE            },
        { "texture_ref",    IdentType::TEXTURE_REF      },
        { "fuzz",           IdentType::FUZZ             },
        { "ior",            IdentType::IOR              },
        { "intensity",      IdentType::INTENSITY        },
        { "shape",          IdentType::SHAPE            },
        { "material_ref",   IdentType::MATERIAL_REF     },
        { "radius",         IdentType::RADIUS           },
        { "y_min",          IdentType::Y_MIN            },
        { "y_max",          IdentType::Y_MAX            },
        { "capped",         IdentType::CAPPED           },
        { "min",            IdentType::MIN              },
        { "max",            IdentType::MAX              },
        { "normal",         IdentType::NORMAL           },
        { "major_radius",   IdentType::MAJOR_RADIUS     },
        { "minor_radius",   IdentType::MINOR_RADIUS     },
        { "translate",      IdentType::TRANSLATE        },
        { "rotate",         IdentType::ROTATE           },
        { "direction",      IdentType::DIRECTION        },
    };
    auto it = m.find(s);
    return it != m.end() ? it->second : IdentType::UNKNOWN;
}


//================================================================//


/// @brief Converts a BlockType to a readable string for logs/errors.
inline std::string logString(BlockType v) {
    switch (v) {
        case BlockType::SETTING:  return "SETTING";
        case BlockType::CAMERA:   return "CAMERA";
        case BlockType::MATERIAL: return "MATERIAL";
        case BlockType::TEXTURE:  return "TEXTURE";
        case BlockType::OBJECT:   return "OBJECT";
        case BlockType::LIGHT:    return "LIGHT";
        default:                  return "UNKNOWN";
    }
}

/// @brief Converts an ObjectType to a readable string for logs/errors.
inline std::string logString(ObjectType v) {
    switch (v) {
        case ObjectType::SPHERE:   return "SPHERE";
        case ObjectType::CYLINDER: return "CYLINDER";
        case ObjectType::CONE:     return "CONE";
        case ObjectType::BOX:      return "BOX";
        case ObjectType::QUAD:     return "QUAD";
        case ObjectType::DISK:     return "DISK";
        case ObjectType::MESH:     return "MESH";
        default:                   return "UNKNOWN";
    }
}

/// @brief Converts a TextureType to a readable string for logs/errors.
inline std::string logString(TextureType v) {
    switch (v) {
        case TextureType::SOLIDCOLOR: return "SOLIDCOLOR";
        case TextureType::IMAGE:      return "IMAGE";
        case TextureType::CHECKER:    return "CHECKER";
        case TextureType::NOISE:      return "NOISE";
        case TextureType::TURBULENCE: return "TURBULENCE";
        case TextureType::MARBLE:     return "MARBLE";
        case TextureType::WARPED:     return "WARPED";
        case TextureType::WOOD:       return "WOOD";
        default:                      return "UNKNOWN";
    }
}

/// @brief Converts a MaterialType to a readable string for logs/errors.
inline std::string logString(MaterialType v) {
    switch (v) {
        case MaterialType::LAMBERTIAN: return "LAMBERTIAN";
        case MaterialType::METALLIC:   return "METALLIC";
        case MaterialType::DIELECTRIC: return "DIELECTRIC";
        case MaterialType::ISOTROPIC:  return "ISOTROPIC";
        case MaterialType::EMISSIVE:   return "EMISSIVE";
        default:                       return "UNKNOWN";
    }
}

/// @brief Converts a LightType to a readable string for logs/errors.
inline std::string logString(LightType v) {
    switch (v) {
        case LightType::POINT:       return "POINT";
        case LightType::DIRECTIONAL: return "DIRECTIONAL";
        default:                     return "UNKNOWN";
    }
}

/// @brief Converts an IdentType to a readable string for logs/errors.
inline std::string logString(IdentType v) {
    switch (v) {
        case IdentType::TYPE:           return "TYPE";
        case IdentType::NAME:           return "NAME";
        case IdentType::FILE:           return "FILE";
        case IdentType::WIDTH:          return "WIDTH";
        case IdentType::HEIGHT:         return "HEIGHT";
        case IdentType::SAMPLES:        return "SAMPLES";
        case IdentType::DEPTH:          return "DEPTH";
        case IdentType::ASPECT_RATIO:   return "ASPECT_RATIO";
        case IdentType::BACKGROUND:     return "BACKGROUND";
        case IdentType::POSITION:       return "POSITION";
        case IdentType::LOOKAT:         return "LOOKAT";
        case IdentType::UP_VIEW:        return "UP_VIEW";
        case IdentType::VFOV:           return "VFOV";
        case IdentType::DEFOCUS_ANGLE:  return "DEFOCUS_ANGLE";
        case IdentType::FOCUS_DISTANCE: return "FOCUS_DISTANCE";
        case IdentType::COLOR:          return "COLOR";
        case IdentType::EVEN:           return "EVEN";
        case IdentType::ODD:            return "ODD";
        case IdentType::SCALE:          return "SCALE";
        case IdentType::TEXTURE_REF:    return "TEXTURE_REF";
        case IdentType::FUZZ:           return "FUZZ";
        case IdentType::IOR:            return "IOR";
        case IdentType::INTENSITY:      return "INTENSITY";
        case IdentType::SHAPE:          return "SHAPE";
        case IdentType::MATERIAL_REF:   return "MATERIAL_REF";
        case IdentType::RADIUS:         return "RADIUS";
        case IdentType::Y_MIN:          return "Y_MIN";
        case IdentType::Y_MAX:          return "Y_MAX";
        case IdentType::CAPPED:         return "CAPPED";
        case IdentType::MIN:            return "MIN";
        case IdentType::MAX:            return "MAX";
        case IdentType::NORMAL:         return "NORMAL";
        case IdentType::MAJOR_RADIUS:   return "MAJOR_RADIUS";
        case IdentType::MINOR_RADIUS:   return "MINOR_RADIUS";
        case IdentType::TRANSLATE:      return "TRANSLATE";
        case IdentType::ROTATE:         return "ROTATE";
        case IdentType::DIRECTION:      return "DIRECTION";
        default:                        return "UNKNOWN";
    }
}

#endif