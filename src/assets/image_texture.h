#ifndef IMAGE_TEXTURE_H
#define IMAGE_TEXTURE_H

#include "texture.h"
#include "image.h"

class ImageTexture : public Texture {
    public:
        ImageTexture(const std::string& _path);
        ImageTexture(const Image* _img, const std::string& name = "");
        Color sample(const Vec2d& uv, const Point3d& p) const override;
        std::string serializeLNT() const override;

    private:
        std::string path;
        const Image* tex_img;
        Image owned;
};

#endif
