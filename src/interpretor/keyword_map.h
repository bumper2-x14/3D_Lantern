#ifndef KEYWORD_MAP_H
#define KEYWORD_MAP_H

#include <unordered_map>
#include <string>

enum class BlockType {
    SETTING, CAMERA, MATERIAL, TEXTURE, OBJECT, LIGHT, UNKNOWN
};

enum class ObjectType {
    SPHERE, CYLINDER, CONE, BOX, QUAD, DISK, MESH, UNKNOWN
};

enum class TextureType {
    SOLIDCOLOR, IMAGE, CHECKER, PERLIN, UNKNOWN
};

enum class MaterialType {
    LAMBERTIAN, METALLIC, DIELECTRIC, ISOTROPIC, EMISSIVE, UNKNOWN
};

enum class LightType {
    POINT, DIRECTIONAL, UNKNOWN
};

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

inline ObjectType convertToObjectType(const std::string& s) {
    static const std::unordered_map<std::string, ObjectType> m {
        { "sphere",   ObjectType::SPHERE   },
        { "cylinder", ObjectType::CYLINDER },
        { "cone",     ObjectType::CONE     },
        { "box",      ObjectType::BOX      },
        { "quad",     ObjectType::QUAD     },
        { "disk",     ObjectType::DISK     },
        { "mesh",     ObjectType::MESH     }
    };
    auto it = m.find(s);
    return it != m.end() ? it->second : ObjectType::UNKNOWN;
}

inline TextureType convertToTextureType(const std::string& s) {
    static const std::unordered_map<std::string, TextureType> m {
        { "solidcolor", TextureType::SOLIDCOLOR },
        { "image",      TextureType::IMAGE      },
        { "checker",    TextureType::CHECKER    },
        { "perlin",     TextureType::PERLIN     }
    };
    auto it = m.find(s);
    return it != m.end() ? it->second : TextureType::UNKNOWN;
}

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

inline LightType convertToLightType(const std::string& s) {
    static const std::unordered_map<std::string, LightType> m {
        { "point",       LightType::POINT       },
        { "directional", LightType::DIRECTIONAL }
    };
    auto it = m.find(s);
    return it != m.end() ? it->second : LightType::UNKNOWN;
}

inline IdentType convertToIdentType(const std::string& s) {
    static const std::unordered_map<std::string, IdentType> m {
        { "type",           IdentType::TYPE           },
        { "name",           IdentType::NAME           },
        { "file",           IdentType::FILE           },
        { "width",          IdentType::WIDTH          },
        { "height",         IdentType::HEIGHT         },
        { "samples",        IdentType::SAMPLES        },
        { "depth",          IdentType::DEPTH          },
        { "aspect_ratio",   IdentType::ASPECT_RATIO   },
        { "background",     IdentType::BACKGROUND     },
        { "position",       IdentType::POSITION       },
        { "lookat",         IdentType::LOOKAT         },
        { "up_view",        IdentType::UP_VIEW        },
        { "vfov",           IdentType::VFOV           },
        { "defocus_angle",  IdentType::DEFOCUS_ANGLE  },
        { "focus_distance", IdentType::FOCUS_DISTANCE },
        { "color",          IdentType::COLOR          },
        { "even",           IdentType::EVEN           },
        { "odd",            IdentType::ODD            },
        { "scale",          IdentType::SCALE          },
        { "texture",        IdentType::TEXTURE_REF    },
        { "fuzz",           IdentType::FUZZ           },
        { "ior",            IdentType::IOR            },
        { "intensity",      IdentType::INTENSITY      },
        { "shape",          IdentType::SHAPE          },
        { "material",       IdentType::MATERIAL_REF   },
        { "radius",         IdentType::RADIUS         },
        { "y_min",          IdentType::Y_MIN          },
        { "y_max",          IdentType::Y_MAX          },
        { "capped",         IdentType::CAPPED         },
        { "min",            IdentType::MIN            },
        { "max",            IdentType::MAX            },
        { "normal",         IdentType::NORMAL         },
        { "major_radius",   IdentType::MAJOR_RADIUS   },
        { "minor_radius",   IdentType::MINOR_RADIUS   },
        { "translate",      IdentType::TRANSLATE      },
        { "rotate",         IdentType::ROTATE         },
        { "direction",      IdentType::DIRECTION      },
    };
    auto it = m.find(s);
    return it != m.end() ? it->second : IdentType::UNKNOWN;
}

#endif