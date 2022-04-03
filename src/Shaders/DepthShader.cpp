#include "DepthShader.h"

DepthShader::DepthShader(Model &model, float depth)
    : IShader{model}, m_depth{depth} {}

Vec4f DepthShader::vertex(const int face_idx, const int vertex_idx)
{
    Vec4f gl_Vertex = embed<4>(m_model.vert(face_idx, vertex_idx));
    gl_Vertex = Viewport * Projection * ModelView * gl_Vertex;
    m_varying_zbuffer.set_column(vertex_idx, proj<3>(gl_Vertex / gl_Vertex[3]));
    return gl_Vertex;
}

bool DepthShader::fragment(const Vec3f bar, TGAColor &color)
{
    Vec3f point = m_varying_zbuffer * bar;
    color = TGAColor(255, 255, 255) * (point.z / m_depth);
    return false;
}