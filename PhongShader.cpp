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
    Vec3f normal = proj<3>(m_uniform_PMVIT * embed<4>(m_model.normal(uv))).normalized(); 
    Vec3f light = proj<3>(m_uniform_PMV * embed<4>(m_light_dir)).normalized();
    Vec3f reflection = (2.f * normal * normal * light - light).normalized();  // phong reflection formula
    float specular = pow(std::max(reflection.z, 0.0f), m_model.specular(uv));
    float diffuse = std::max(0.f, normal * light);
    color = m_model.diffuse(uv);
    for (int i = 0; i < 3; i++) 
        color[i] = std::min<float>(2 + color[i] * (0.8f * diffuse + 5.0f * specular), 255);
    return false;
    }