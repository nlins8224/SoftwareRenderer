#include <vector>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <iostream>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"


const int WIDTH = 800;
const int HEIGHT = 800;
const int DEPTH = 255;
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0,   255, 0,   255);
Model *model = NULL;
int *zbuffer = NULL;

Vec3f lightDir(0, 0, -1);
Vec3f camera(0, 0, 3);

Vec3f matrixToVector(Matrix m) {
	return Vec3f(m[0][0] / m[3][0], m[1][0] / m[3][0], m[2][0] / m[3][0]);
}

Matrix vectorToMatrix(Vec3f v) {
	Matrix m(4, 1);
	m[0][0] = v.x;
	m[1][0] = v.y;
	m[2][0] = v.z;
	m[3][0] = 1.f;
	return m;
}

Matrix viewport(int x, int y, int w, int h, int d) {
	Matrix m = Matrix::identity(4);
	// Scale
	m[0][0] = w / 2.f;
	m[1][1] = h / 2.f;
	m[2][2] = d / 2.f;
	// Translate
	m[0][3] = x + w / 2.f;
	m[1][3] = y + h / 2.f;
	m[2][3] = d / 2.f;
	return m;
}

/* This function draws a line by iterating over x axis. x1, x2 are swapped if x1 < x0, because of loop conditional. 
If height is greater than width, then x and y are transposed and de-transposed to avoid line cutting (because of loop conditional). */
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
	bool transposed = false; 
		/* width < height ? */
		if (std::abs(x0 - x1) < std::abs(y0 - y1)) { // transpose
			std::swap(x0, y0); 
			std::swap(x1, y1); 
			transposed = true; 
		} 


	if (x1 < x0){
		std::swap(x0, x1);
		std::swap(y0, y1);
	} 

	float dx = x1 - x0;
	/* This could be further optimized by removing divisions */
	for (int x = x0; x <= x1; x++) {
		float t = (x - x0) / dx;
        int y = y0 * (1. - t) + y1 * t; 
        if (transposed) { 
            image.set(y, x, color); // de−transpose 
        } else { 
            image.set(x, y, color); 
        } 
	}
}

/* This function draws a line by iterating over x axis. x1, x2 are swapped if x1 < x0, because of loop conditional. 
If height is greater than width, then x and y are transposed and de-transposed to avoid line cutting (because of loop conditional). */
void line(Vec2i p0, Vec2i p1, TGAImage &image, TGAColor color) {
	bool transposed = false; 

	uint width = std::abs(p0.x - p1.x);
	uint height = std::abs(p0.y - p1.y);

	if (width < height) { // transpose
		std::swap(p0.x, p0.y); 
		std::swap(p1.x, p1.y); 
		transposed = true; 
	} 


	if (p1.x < p0.x){
		std::swap(p0, p1);
	} 

	float dx = p1.x - p0.x;
	/* This could be further optimized by removing divisions */
	for (int x = p0.x; x <= p1.x; x++) {
		float t = (x - p0.x) / dx;
        int y = p0.y * (1. - t) + p1.y * t; 
        if (transposed) { 
            image.set(y, x, color); // de−transpose 
        } else { 
            image.set(x, y, color); 
        } 
	}
}


#if 1

void triangle(Vec3i t0, Vec3i t1, Vec3i t2, Vec2i uv0, Vec2i uv1, Vec2i uv2, TGAImage &image, float intensity, int *zbuffer) {
    if (t0.y==t1.y && t0.y==t2.y) return; // i dont care about degenerate triangles
    if (t0.y>t1.y) { std::swap(t0, t1); std::swap(uv0, uv1); }
    if (t0.y>t2.y) { std::swap(t0, t2); std::swap(uv0, uv2); }
    if (t1.y>t2.y) { std::swap(t1, t2); std::swap(uv1, uv2); }

    int total_height = t2.y-t0.y;
    for (int i=0; i<total_height; i++) {
        bool second_half = i>t1.y-t0.y || t1.y==t0.y;
        int segment_height = second_half ? t2.y-t1.y : t1.y-t0.y;
        float alpha = (float)i/total_height;
        float beta  = (float)(i-(second_half ? t1.y-t0.y : 0))/segment_height; // be careful: with above conditions no division by zero here
        Vec3i A   =               t0  + Vec3f(t2-t0  )*alpha;
        Vec3i B   = second_half ? t1  + Vec3f(t2-t1  )*beta : t0  + Vec3f(t1-t0  )*beta;
        Vec2i uvA =               uv0 +      (uv2-uv0)*alpha;
        Vec2i uvB = second_half ? uv1 +      (uv2-uv1)*beta : uv0 +      (uv1-uv0)*beta;
        if (A.x>B.x) { std::swap(A, B); std::swap(uvA, uvB); }
        for (int j=A.x; j<=B.x; j++) {
            float phi = B.x==A.x ? 1. : (float)(j-A.x)/(float)(B.x-A.x);
            Vec3i   P = Vec3f(A) + Vec3f(B-A)*phi;
            Vec2i uvP =     uvA +   (uvB-uvA)*phi;
            int idx = P.x+P.y*WIDTH;
            if (zbuffer[idx]<P.z) {
                zbuffer[idx] = P.z;
                TGAColor color = model->diffuse(uvP);
                image.set(P.x, P.y, TGAColor(color.r*intensity, color.g*intensity, color.b*intensity));
            }
        }
    }
}

#else

/* */
Vec3f barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P) {
	
	/*
	 Cross product of V1 and V2 gives barycentric coordinates parameters
	 AB, AC, PA are vectors

	 From barycentric coordinates definition, we know that:
     1. P(u, v, z) = zA + uB + vC
	 2. z + u + v = 1
	 thus: 	
	 P(u, v) = (1 - u - v)A + uB + vC
	 then:
	 P(u, v) = A - uA + uB - vA + vC
	 P(u, v) = A + uAB + vAC
	 A - P + uAB + vAC = 0
	 PA + uAB + vAC = 0
	*/
	Vec3f V1 = Vec3f(
		C.x - A.x,     // AC_x
	    B.x - A.x,     // AB_x
		A.x - P.x	   // PA_x
	);

	Vec3f V2 = Vec3f(
		C.y - A.y,     // AC_y
	    B.y - A.y,     // AB_y
		A.y - P.y	   // PA_y
	);

	Vec3f u = V1 ^ V2;

	// degenerate case
	if (std::abs(u.z) < 1) return Vec3f(-1, 1, 1);
	
	// (1 - u - v, u, v)
	return Vec3f(1.f - (u.x + u.y) / u.z, 
	             u.y / u.z, 
				 u.x / u.z);
}

/* Pixel is inside triangle if barycentric coordinates parameters 0 <= u, v, z <= 1 */
// bool inTriangle(Vec2i *pts, Vec2i P) {
// 	Vec3f bParams = barycentric(pts, P);
// 	return bParams.x >= 0 && bParams.y >= 0 && bParams.z >= 0;
// }

/* 
For each pixel in bounding box if pixel is inside triangle then color that pixel
*/
void triangle(Vec3f *pts, float *zbuffer, TGAImage &image, TGAColor color) {
	Vec2f clamp(image.get_width() - 1, image.get_height() - 1);
	Vec2f boundingBoxMax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
	Vec2f boundingBoxMin(std::numeric_limits<float>::max(),  std::numeric_limits<float>::max());

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			boundingBoxMin[j] = std::max(0.f, std::min(boundingBoxMin[j], pts[i][j]));
            boundingBoxMax[j] = std::min(clamp[j], std::max(boundingBoxMax[j], pts[i][j]));
		}
	}

	Vec3f P;
	for (P.x = boundingBoxMin.x; P.x <= boundingBoxMax.x; P.x++) {
		for (P.y = boundingBoxMin.y; P.y <= boundingBoxMax.y; P.y++) {
			Vec3f bParams = barycentric(pts[0], pts[1], pts[2], P);
			if (bParams.x < 0 || bParams.y < 0 || bParams.z < 0) 
				continue;
			P.z = 0;
			for (int i = 0; i < 3; i++) {
				P.z += pts[i][2] * bParams[i];
			}
			if (zbuffer[int(P.x + P.y * WIDTH)] < P.z) {
				zbuffer[int(P.x + P.y * WIDTH)] = P.z;
				image.set(P.x, P.y, color);
			}
		}
	}
}

#endif

Vec3f worldToScreen(Vec3f v, uint width, uint height) {
    return Vec3f(int((v.x + 1.) * width / 2. + .5), int((v.y + 1.) * height / 2. + .5), v.z);
}

void render(Model *model, int width, int height, int depth, TGAImage &image, float scale) {
	int *zbuffer = new int[width * height];
	for (int i = 0; i < width * height; i++) {
        zbuffer[i] = std::numeric_limits<int>::min();
    }

	Matrix Projection = Matrix::identity(4);
    Matrix ViewPort   = viewport(width / scale, height / scale, width * 3 / 4, height * 3 / 4, depth);
    Projection[3][2] = -1.f / camera.z;

	for (int i = 0; i < model->nfaces(); i++) {
		std::vector<int> face = model->face(i);
	
        Vec3f screenCoords[3];
		Vec3f worldCoords[3];
        for (int j = 0; j < 3; j++) {
			Vec3f v = model->vert(face[j]);
			screenCoords[j] = matrixToVector(ViewPort * Projection * vectorToMatrix(v));
			worldCoords[j] = v;
		}
		Vec3f n = (worldCoords[2] - worldCoords[0]) ^ (worldCoords[1] - worldCoords[0]);
		n.normalize();
		float intensity = n * lightDir;
		if (intensity > 0) {
			Vec2i uv[3];
            for (int k=0; k<3; k++) {
                uv[k] = model->uv(i, k);
            }
            triangle(screenCoords[0], screenCoords[1], screenCoords[2], uv[0], uv[1], uv[2], image, intensity, zbuffer);
		}
	}
}

void init() {
	model = new Model("obj/african_head.obj");
}


int main(int argc, char** argv) {
	init();
	TGAImage image(WIDTH, HEIGHT, TGAImage::RGB);
	render(model, WIDTH, HEIGHT, DEPTH, image, 8);
	image.flip_vertically(); // origin at the left bottom corner of the image
	image.write_tga_file("output2.tga");
	delete model;
	delete [] zbuffer;
	return 0;
}

