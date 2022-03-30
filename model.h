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
    std::vector<Vec3f> m_verts;
    std::vector<std::vector<Vec3i> > m_faces; // attention, this Vec3i means vertex/uv/normal
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

