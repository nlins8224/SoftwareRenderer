#include "tgaimage.h"
#include "model.h"
#include "geometry.h"
#include <algorithm>


const int WIDTH = 800;
const int HEIGHT = 800;
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
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

void render(Model *model, int width, int height, float scale) {
	TGAImage image(width, height, TGAImage::RGB);

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

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output2.tga");

}

void init() {
	model = new Model("obj/tree.obj");
}


int main(int argc, char** argv) {
	init();
	render(model, WIDTH, HEIGHT, 4);
	delete model;
	return 0;
}

