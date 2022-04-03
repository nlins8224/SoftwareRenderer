#include <iostream>
#include <fstream>
#include <sstream>
#include "model.h"

Model::Model(std::string filename)
:m_verts(), m_faces(), m_normals(), m_textures_uv(), m_diffusemap(), m_normalmap(), m_specularmap() {
    parse(filename);
    std::cerr << "# v# " << m_verts.size() << " f# "  << m_faces.size() << " vt# " << m_textures_uv.size() << " vn# " << m_normals.size() << std::endl;
    load_texture(filename, "_diffuse.tga", m_diffusemap);
    load_texture(filename, "_nm.tga",      m_normalmap);
    load_texture(filename, "_spec.tga",    m_specularmap);
}

Model::~Model() {}

void Model::parse(std::string filename) {
    std::ifstream in;
    in.open (filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line);
        if (line.compare(0, 2, "v ") == 0) {
            parse_v(iss);
        } else if (line.compare(0, 3, "vn ") == 0) {
            parse_vn(iss);
        } else if (line.compare(0, 3, "vt ") == 0) {
            parse_vt(iss);
        } else if (line.compare(0, 2, "f ") == 0) {
            parse_f(iss);
        }
    }
}

void Model::parse_v(std::istringstream& stream) {
    char trash;
    stream >> trash;
    Vec3f v;
    for (int i : {0, 1, 2}) 
        stream >> v[i];
    m_verts.push_back(v);
}

void Model::parse_vn(std::istringstream& stream) {
    char trash;
    stream >> trash >> trash;
    Vec3f n;
    for (int i : {0, 1, 2})
        stream >> n[i];
    m_normals.push_back(n);
}

void Model::parse_vt(std::istringstream& stream) {
    char trash;
    stream >> trash >> trash;
    Vec2f uv;
    for (int i : {0, 1}) 
        stream >> uv[i];
    m_textures_uv.push_back(uv);
}

void Model::parse_f(std::istringstream& stream) {
    char trash;

    std::vector<Vec3i> faces;
    Vec3i face;
    stream >> trash;
    while (stream >> face[0] >> trash >> face[1] >> trash >> face[2]) {
        for (int i : {0, 1, 2}) 
            face[i]--; // in wavefront obj all indices start at 1
        faces.push_back(face);
    }
    m_faces.push_back(faces);
}

int Model::nverts() {
    return static_cast<int>(m_verts.size());
}

int Model::nfaces() {
    return static_cast<int>(m_faces.size());
}

std::vector<int> Model::face(int idx) {
    std::vector<int> face;
    for (int i = 0; i < static_cast<int>(m_faces[idx].size()); i++) 
        face.push_back(m_faces[idx][i][0]);
    return face;
}

Vec3f Model::vert(int idx) {
    return m_verts[idx];
}

/* return xyz geometric coordinates of vertex_idx vertex from face_idx face */
Vec3f Model::vert(int face_idx, int vertex_idx) {
    return m_verts[m_faces[face_idx][vertex_idx][0]];
}

/* return uv texture coordinates of vertex_idx vertex from face_idx face */
Vec2f Model::uv(int face_idx, int vertex_idx) {
    return m_textures_uv[m_faces[face_idx][vertex_idx][1]];
}

/* return xyz normal coordinates of vertex_idx vertex from face_idx face */
Vec3f Model::normal(int face_idx, int vertex_idx) {
    int idx = m_faces[face_idx][vertex_idx][2];
    m_normals[idx] = m_normals[idx].normalized();
    return m_normals[idx];
}

void Model::load_texture(std::string filename, std::string suffix, TGAImage &img) {
    std::string texfile(filename);
    size_t dot = texfile.find_last_of(".");
    if (dot != std::string::npos) {
        texfile = texfile.substr(0, dot) + std::string(suffix);
        std::cerr << "texture file " << texfile << " loading " << (img.read_tga_file(texfile.c_str()) ? "ok" : "failed") << std::endl;
        img.flip_vertically();
    }
}

TGAColor Model::diffuse(Vec2f textures_uv) {
    Vec2i uv(textures_uv[0] * m_diffusemap.get_width(), textures_uv[1] * m_diffusemap.get_height());
    return m_diffusemap.get(uv[0], uv[1]);
}

Vec3f Model::normal(Vec2f textures_uv) {
    Vec2i uv(textures_uv[0] * m_normalmap.get_width(), textures_uv[1] * m_normalmap.get_height());
    TGAColor c = m_normalmap.get(uv[0], uv[1]);
    Vec3f result;
    for (int i : {0, 1, 2})
        result[2 - i] = static_cast<float>(c[i]) / 255.f * 2.f - 1.f;
    return result;
}

float Model::specular(Vec2f textures_uv) {
    Vec2i uv(textures_uv[0] * m_specularmap.get_width(), textures_uv[1] * m_specularmap.get_height());
    return m_specularmap.get(uv[0], uv[1])[0] / 1.f;
}
