#include <iomanip>

#include "checker_texture.h"
#include "color_texture.h"


CheckerTexture::CheckerTexture(Texture* _even, Texture* _odd, double _scale) :
    even(_even), odd(_odd), scale(_scale) {}

        
CheckerTexture::CheckerTexture(const Color& _even, const Color& _odd, double _scale) :
    even(new ColorTexture(_even)), odd(new ColorTexture(_odd)), scale(_scale) {}
        
CheckerTexture::~CheckerTexture() {
    delete even; 
    delete odd;
}

Color CheckerTexture::sample(const Vec2d& uv, const Point3d& p) const {
    int x = int(std::floor(scale * p.x));
    int y = int(std::floor(scale * p.y));
    int z = int(std::floor(scale * p.z));
    bool is_even = ((x + y + z) % 2) == 0;

    return is_even ? even->sample(uv, p) : odd->sample(uv, p);
}

// Rasterize over a range that shows several checker squares
// scale=10 means 10 checks per world unit — show 2 world units = 20 checks
Point3d CheckerTexture::uvToWorldConverter(const Vec2d& uv) const {
    double range = 4.0 / scale;   // always show ~4 checker squares
    return Point3d(
        (uv.x - 0.5) * range,
        0.0,
        (uv.y - 0.5) * range
    );
}

std::string CheckerTexture::serializeLNT() const {
    const ColorTexture* e = dynamic_cast<const ColorTexture*>(even);
    const ColorTexture* o = dynamic_cast<const ColorTexture*>(odd);

    if (!e || !o) return "";  // nested non-color textures — not expressible in .lnt

    Color ec = e->getColor();
    Color oc = o->getColor();

    std::ostringstream s;
    s << std::fixed << std::setprecision(4);
    s << "    type checker\n"
      << "    even " << ec.r << " " << ec.g << " " << ec.b << "\n"
      << "    odd "  << oc.r << " " << oc.g << " " << oc.b << "\n"
      << "    scale " << scale << "\n";
    return s.str();
}