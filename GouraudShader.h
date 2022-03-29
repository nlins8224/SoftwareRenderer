#ifndef GOURAUDSHADER_H
#define GOURAUDSHADER_H
#include "model.h"
#include "Vec.h"
#include "gl.h"
#include "IShader.h"

class GouraudShader : public IShader {
    private:
    Vec3f m_light_dir {};
    Vec3f m_varying_intensity {};

    public:
    GouraudShader(Model& model, Vec3f light_dir);
    Vec4f vertex(const int iface, const int nthvert) override;
    bool fragment(const Vec3f bar, TGAColor& color)  override;
};
#endif // GOURAUDSHADER_H