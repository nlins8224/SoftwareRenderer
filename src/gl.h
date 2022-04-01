#ifndef __GL_H__
#define __GL_H__

#include "tgaimage.h"
#include "./Math/Vec.h"
#include "./Math/Matrix.h"
#include "./Shaders/IShader.h"

extern Matrix4f ModelView;
extern Matrix4f Viewport;
extern Matrix4f Projection;

void viewport(int x, int y, int w, int h, int d);
void projection(float coeff=0.f); // coeff = -1/c
void lookat(Vec3f eye, Vec3f center, Vec3f up);

void triangle(Vec4f *pts, IShader &shader, TGAImage &image, TGAImage &zbuffer);

#endif // __GL_H__