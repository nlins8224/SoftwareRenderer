#include "GouraudShader.h"

GouraudShader::GouraudShader(Model* model, Vec3f light_dir): m_model{model}, m_light_dir{light_dir} {};

Vec4f GouraudShader::vertex(int iface, int nthvert) {
    Vec4f gl_Vertex = embed<4>(m_model->vert(iface, nthvert)); // read the vertex from .obj file
    gl_Vertex = Viewport * Projection * ModelView * gl_Vertex;     // transform it to screen coordinates
    varying_intensity[nthvert] = std::max(0.f, m_model->normal(iface, nthvert)*m_light_dir); // get diffuse lighting intensity
    return gl_Vertex;
}

bool GouraudShader::fragment(Vec3f bar, TGAColor &color) {
        float intensity = varying_intensity*bar;   // interpolate intensity for the current pixel
        color = TGAColor(255, 255, 255)*intensity; // well duh
        return false;                              // no, we do not discard this pixel
}