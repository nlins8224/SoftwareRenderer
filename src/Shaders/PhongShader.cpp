#include "PhongShader.h"

PhongShader::PhongShader(Model& model, Vec3f light_dir)
    : IShader{model}, m_light_dir{light_dir} {}

Vec4f PhongShader::vertex(const int face_idx, const int vertex_idx) {
        m_varying_uv.set_column(vertex_idx, m_model.uv(face_idx, vertex_idx));
        Vec4f gl_Vertex = embed<4>(m_model.vert(face_idx, vertex_idx));
        return Viewport * Projection * ModelView * gl_Vertex;
    }

bool PhongShader::fragment(const Vec3f bar, TGAColor& color) {
    Vec2f uv = m_varying_uv * bar;
    Vec3f normal = m_model.normal(uv);
    Vec3f reflected_light = normal * normal * m_light_dir * 2.f - m_light_dir; // light reflection formula
    float specular = pow(std::max(reflected_light.z / reflected_light.norm(), 0.0f), m_model.specular(uv));
    float diffuse = std::max(0.f, normal * m_light_dir);
    float ambient = 5.0f;

    float coeff = normal * m_light_dir;

    TGAColor diff = m_model.diffuse(uv) * coeff;
    color = m_model.diffuse(uv);
    for (int i = 0; i < 3; i++) 
        color[i] = std::min<float>(ambient + diff[i] * (diffuse + specular), 255.f);
    return false;
    }