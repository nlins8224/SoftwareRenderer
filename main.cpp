#include "tgaimage.h"
#include "model.h"
#include "geometry.h"
#include <algorithm>


const int WIDTH = 800;
const int HEIGHT = 800;
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0,   255, 0,   255);
Model *model = NULL;

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

/* Maybe could be optimized to O(N*logN) */
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




void render(Model *model, int width, int height, TGAImage &image, float scale) {

	for (int i = 0; i < model->nfaces(); i++) {
		std::vector<int> face = model->face(i);
		for (int j = 0; j < 3; j++) { 
        	Vec3f v0 = model->vert(face[j]); 
        	Vec3f v1 = model->vert(face[(j + 1) % 3]); 
        	int x0 = (v0.x + 1.) * width / scale; 
        	int y0 = (v0.y + 1.) * height / scale; 
        	int x1 = (v1.x + 1.) * width / scale; 
        	int y1 = (v1.y + 1.) * height / scale; 
        	line(x0, y0, x1, y1, image, white); 
    	} 
	}
}

void init() {
	model = new Model("obj/tree.obj");
}


int main(int argc, char** argv) {
	init();
	TGAImage image(WIDTH, HEIGHT, TGAImage::RGB);
	//render(model, WIDTH, HEIGHT, image, 4);
	Vec2i t0(10, 70);
	Vec2i t1(50, 160);
	Vec2i t2(70, 80);
	triangle(t0, t1, t2, image, red);
	image.flip_vertically(); // origin at the left bottom corner of the image
	image.write_tga_file("output2.tga");
	delete model;
	return 0;
}

