#ifndef GOURAUDSHADER_H
#define GOURAUDSHADER_H
#include "model.h"
#include "geometry.h"
#include "gl.h"

class GouraudShader : public IShader {
    private:
    Model* m_model {nullptr};
    Vec3f m_light_dir {};
    Vec3f varying_intensity {}; // written by vertex shader, read by fragment shader

    public:
    GouraudShader(Model* model, Vec3f light_dir);
    Vec4f vertex(int iface, int nthvert);
    bool fragment(Vec3f bar, TGAColor &color);
};
#endif // GOURAUDSHADER_H