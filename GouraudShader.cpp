#include "GouraudShader.h"

GouraudShader::GouraudShader(Model& model, Vec3f light_dir)
    : IShader{model}, m_light_dir{light_dir} {}

Vec4f GouraudShader::vertex(const int face_idx, const int vertex_idx) {
    Vec4f gl_Vertex = embed<4>(m_model.vert(face_idx, vertex_idx)); // read the vertex from .obj file
    gl_Vertex = Viewport * Projection * ModelView * gl_Vertex;     // transform it to screen coordinates
    m_varying_intensity[vertex_idx] = std::max(0.f, m_model.normal(face_idx, vertex_idx)*m_light_dir); // get diffuse lighting intensity
    return gl_Vertex;
}

bool GouraudShader::fragment(const Vec3f bar, TGAColor& color) {
        float intensity = m_varying_intensity * bar;   
        color = TGAColor(255, 255, 255) * intensity;
        return false;                             
}