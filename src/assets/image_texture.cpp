#include "image_texture.h"

ImageTexture::ImageTexture(const std::string& _path) :
    path(_path), owned(Image::load(_path)), tex_img(&owned) {}

ImageTexture::ImageTexture(const Image* _img, const std::string& _name) : path(_name), tex_img(_img) {}

Color ImageTexture::sample(const Vec2d& uv, const Point3d& p) const {
    if (tex_img->height <= 0) return Color(0,1,1);

    double u = Intervald(0 , 1).clamp(uv.x);
    double v = 1.0 - Intervald(0 , 1).clamp(uv.y);

    int i = int(u * tex_img->width);
    int j = int(v * tex_img->height);
    unsigned char* pix = tex_img->pixelData(i, j);

    double c_scale = 1.0 / 255.0;
    return Color(pix[0] * c_scale, pix[1] * c_scale, pix[2] * c_scale);
}

std::string ImageTexture::serializeLNT() const {
    return "    type image\n    file \"" + path + "\"\n";
}