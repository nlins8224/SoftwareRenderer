#include "GouraudShader.h"

GouraudShader::GouraudShader(Model& model, Vec3f light_dir)
    : IShader{model}, m_light_dir{light_dir} {}

Vec4f GouraudShader::vertex(const int iface, const int nthvert) {
    Vec4f gl_Vertex = embed<4>(m_model.vert(iface, nthvert)); // read the vertex from .obj file
    gl_Vertex = Viewport * Projection * ModelView * gl_Vertex;     // transform it to screen coordinates
    m_varying_intensity[nthvert] = std::max(0.f, m_model.normal(iface, nthvert)*m_light_dir); // get diffuse lighting intensity
    return gl_Vertex;
}

bool GouraudShader::fragment(const Vec3f bar, TGAColor& color) {
        float intensity = m_varying_intensity * bar;   
        color = TGAColor(255, 255, 255) * intensity;
        return false;                             
}