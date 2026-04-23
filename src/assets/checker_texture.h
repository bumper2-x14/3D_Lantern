#ifndef CHECKER_TEXTURE_H
#define CHECKER_TEXTURE_H

#include "texture.h"

class CheckerTexture : public Texture {
    public:
        CheckerTexture(Texture* _even, Texture* _odd, double _scale);
        CheckerTexture(const Color& _even, const Color& _odd, double _scale);
        ~CheckerTexture();
        Color sample(const Vec2d& uv, const Point3d& p) const override;
        Point3d uvToWorldConverter(const Vec2d& uv) const override;

        std::string serializeLNT() const override;

    private:
        Texture* even;
        Texture* odd;
        double scale;
};

#endif