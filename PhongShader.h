#ifndef PHONG_SHADER_H
#define PHONG_SHADER_H
#include "IShader.h"
#include "Vec.h"
#include "Matrix.h"
#include "model.h"
#include "gl.h"

class PhongShader : public IShader {
    private: 
    Vec3f m_light_dir {};
    Vec3f m_varying_intensity{};
    Matrix<2, 3, float> m_varying_uv{};

    public:
    PhongShader(Model& model, Vec3f light_dir);
    Vec4f vertex(const int face_idx, const int vertex_idx) override;
    bool fragment(const Vec3f bar, TGAColor& color)  override;
};

#endif // PHONG_SHADER_H
