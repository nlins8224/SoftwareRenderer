#ifndef __MODEL_H__
#define __MODEL_H__
#include <vector>
#include <string>
#include "./Math/Vec.h"
#include "./Math/Matrix.h"
#include "tgaimage.h"

class Model {
private:
    /* xyz coordinates of vertex */
    std::vector<Vec3f> m_verts{};
    /* 
    m_faces[face_index][vertex_index][vertex_type], 
    Vec3i means vertex/uv/normal
    for example m_faces[1][2][1] means second face, third vertex, uv 
    */
    std::vector<std::vector<Vec3i> > m_faces;
    std::vector<Vec3f> m_normals;
    std::vector<Vec2f> m_textures_uv;
    TGAImage m_diffusemap;
    TGAImage m_normalmap;
    TGAImage m_specularmap;
    void load_texture(std::string filename, std::string suffix, TGAImage &img);
    void parse(std::string filename);
    void parse_v(std::istringstream& stream);
    void parse_vn(std::istringstream& stream);
    void parse_vt(std::istringstream& stream);
    void parse_f(std::istringstream& stream);
public:
    Model(std::string filename);
    ~Model();
    int nverts();
    int nfaces();
    Vec3f normal(int face_idx, int vertex_idx);
    Vec3f normal(Vec2f textures_uv);
    Vec3f vert(int idx);
    Vec3f vert(int face_idx, int vertex_idx);
    Vec2f uv(int face_idx, int vertex_idx);
    TGAColor diffuse(Vec2f textures_uv);
    float specular(Vec2f textures_uv);
    std::vector<int> face(int idx);
};
#endif //__MODEL_H__

