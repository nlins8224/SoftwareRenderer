#ifndef TEXTURED_SHADER_H
#define TEXTURED_SHADER_H

#include "IShader.h"
#include "Vector.h"
#include "Matrix.h"

#include <iostream> 
class TexturedShader : public IShader {
    private: 
    Vec3f m_varying_intensity{};
    Matrix<2, 3, float> m_varying_uv{};

    Vec4f vertex(int iface, int nthvert) override {
        m_varying_uv.set_column(nthvert, model->uv(iface, nthvert));
    }
}



#endif // TEXTURED_SHADER_H
