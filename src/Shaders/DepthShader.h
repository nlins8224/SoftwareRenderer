#ifndef DEPTH_SHADER_H
#define DEPTH_SHADER_H
#include "IShader.h"
#include "../Math/Vec.h"
#include "../Math/Matrix.h"
#include "../model.h"
#include "../gl.h"

class DepthShader : public IShader
{
private:
    Matrix<3, 3, float> m_varying_zbuffer{};
    float m_depth{};

public:
    DepthShader(Model &model, float depth);
    Vec4f vertex(const int face_idx, const int vertex_idx) override;
    bool fragment(const Vec3f bar, TGAColor &color) override;
};

#endif // DEPTH_SHADER_H
