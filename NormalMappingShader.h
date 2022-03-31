#ifndef NORMAL_MAPPING_SHADER_H
#define NORMAL_MAPPING_SHADER_H
#include "IShader.h"
#include "Vec.h"
#include "Matrix.h"
#include "model.h"
#include "gl.h"

class NormalMappingShader : public IShader {
    private: 
    Vec3f m_light_dir {};
    Vec3f m_varying_intensity{};
    Matrix<2, 3, float> m_varying_uv{};
    Matrix<4, 4, float> m_uniform_PMV;
    Matrix<4, 4, float> m_uniform_PMVIT;

    public:
    NormalMappingShader(Model& model, Vec3f light_dir);
    Vec4f vertex(const int face_idx, const int vertex_idx) override;
    bool fragment(const Vec3f bar, TGAColor& color)  override;

    void set_uniform_PMV(Matrix<4, 4, float> uniform_variable) {
        m_uniform_PMV = uniform_variable;
    }

    void set_uniform_PMVIT(Matrix<4, 4, float> uniform_variable) {
        m_uniform_PMVIT = uniform_variable;
    }
};

#endif // NORMAL_MAPPING_SHADER_H
