#ifndef __GL_H__
#define __GL_H__

#include "tgaimage.h"
#include "geometry.h"

extern Matrix ModelView;
extern Matrix Viewport;
extern Matrix Projection;

void viewport(int x, int y, int w, int h, int d);
void projection(float coeff=0.f); // coeff = -1/c
void lookat(Vec3f eye, Vec3f center, Vec3f up);

// TODO: This should be abstract class
struct IShader {
    virtual ~IShader();
    virtual Vec4f vertex(int iface, int nthvert) = 0;
    virtual bool fragment(Vec3f bar, TGAColor &color) = 0;
};

void triangle(Vec4f *pts, IShader &shader, TGAImage &image, TGAImage &zbuffer);

#endif // __GL_H__