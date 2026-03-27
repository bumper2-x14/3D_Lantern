#include "image_texture.h"

ImageTexture::ImageTexture(const std::string& _path) :
    tex_img(Image::load(_path)) {}

Color ImageTexture::sample(const Vec2d& uv, const Point3d& p) const {
    if (tex_img.height <= 0) return Color(0,1,1);

    double u = Intervald(0 , 1).clamp(uv.x);
    double v = 1.0 - Intervald(0 , 1).clamp(uv.y);

    int i = int(u * tex_img.width);
    int j = int(v * tex_img.height);
    unsigned char* pix = tex_img.pixelData(i, j);

    double c_scale = 1.0 / 255.0;
    return Color(pix[0] * c_scale, pix[1] * c_scale, pix[2] * c_scale);
}