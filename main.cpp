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


#if 0
/*
Maybe this could be optimized to O(N*logN) 
Line sweeping algorithm
 */
void triangle(Vec2i p0, Vec2i p1, Vec2i p2, TGAImage &image, TGAColor color) {
	if (p0.y > p1.y) std::swap(p0, p1);
	if (p0.y > p2.y) std::swap(p0, p2);
	if (p1.y > p2.y) std::swap(p1, p2);

	int total_height = p2.y - p0.y;
	int segment_height = p1.y - p0.y;

	for (int y = p0.y; y <= p1.y; y++) {
		float alpha = (float)(y - p0.y) / total_height;
		float beta = (float)(y - p0.y) / (segment_height + 1);
		Vec2i A = p0 + (p2 - p0) * alpha;
		Vec2i B = p0 + (p1 - p0) * beta;

		if (A.x > B.x) std::swap(A, B);
		for (int j = A.x; j <= B.x; j++) {
			image.set(j, y, color);
		}
	}

	segment_height = p2.y - p1.y;
	for (int y = p1.y; y <= p2.y; y++) {
		float alpha = (float)(y - p0.y) / total_height;
		float beta = (float)(y - p1.y) / (segment_height + 1);
		Vec2i A = p0 + (p2 - p0) * alpha;
		Vec2i B = p1 + (p2 - p1) * beta;

		if (A.x > B.x) std::swap(A, B);
		for (int j = A.x; j <= B.x; j++) {
			image.set(j, y, color);
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
	float *zbuffer = new float[width * height];
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
 	       triangle(screenCoords, zbuffer, image, TGAColor(intensity * 255, intensity * 255, intensity * 255, 255));
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

