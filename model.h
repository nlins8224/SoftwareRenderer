#ifndef __MODEL_H__
#define __MODEL_H__
#include <vector>
#include <string>
#include "Vec.h"
#include "Matrix.h"
#include "tgaimage.h"

//TODO: better names
class Model {
private:
    /* xyz coordinates of vertex */
    std::vector<Vec3f> m_verts;
    /* 
    m_faces[face_index][vertex_index][vertex_type], 
    Vec3i means vertex/uv/normal
    for example m_faces[1][2][1] means second face, third vertex, uv 
    */
    std::vector<std::vector<Vec3i> > m_faces;
    std::vector<Vec3f> m_norms;
    std::vector<Vec2f> m_uv;
    TGAImage m_diffusemap;
    TGAImage m_normalmap;
    TGAImage m_specularmap;
    void load_texture(std::string filename, const char *suffix, TGAImage &img);
public:
    Model(const char *filename);
    ~Model();
    int nverts();
    int nfaces();
    Vec3f normal(int iface, int nthvert);
    Vec3f normal(Vec2f uv);
    Vec3f vert(int i);
    Vec3f vert(int iface, int nthvert);
    Vec2f uv(int iface, int nthvert);
    TGAColor diffuse(Vec2f uv);
    float specular(Vec2f uv);
    std::vector<int> face(int idx);
};
#endif //__MODEL_H__

