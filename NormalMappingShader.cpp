#include "NormalMappingShader.h"

NormalMappingShader::NormalMappingShader(Model& model, Vec3f light_dir)
    : IShader{model}, m_light_dir{light_dir} {}

Vec4f NormalMappingShader::vertex(const int face_idx, const int vertex_idx) {
        m_varying_uv.set_column(vertex_idx, m_model.uv(face_idx, vertex_idx));
        m_varying_intensity[vertex_idx] = std::max(0.f, m_model.normal(face_idx, vertex_idx) * m_light_dir);
        Vec4f gl_Vertex = embed<4>(m_model.vert(face_idx, vertex_idx));
        return Viewport * Projection * ModelView * gl_Vertex;
    }

bool NormalMappingShader::fragment(const Vec3f bar, TGAColor& color) {
    Vec2f uv = m_varying_uv * bar;
    Vec3f normal = proj<3>(m_uniform_PMVIT * embed<4>(m_model.normal(uv))).normalized(); 
    Vec3f light = proj<3>(m_uniform_PMV * embed<4>(m_light_dir)).normalized();
    float intensity = std::max(0.f, normal * light);
    color = m_model.diffuse(uv) * intensity;
    return false;
    }