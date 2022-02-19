#include "tgaimage.h"
#include <algorithm>


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

/* This function draws a line by iterating over x axis. x1, x2 are swapped if x1 < x0, because of loop conditional. 
If height is greater than width, then x and y are transposed and de-transposed to avoid line cutting (because of loop conditional). */
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
	if (x1 < x0){
		std::swap(x0, x1);
		std::swap(y0, y1);
	} 

	bool transposed = false; 
	/* width < height ? */
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) { // transpose
        std::swap(x0, y0); 
        std::swap(x1, y1); 
        transposed = true; 
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

int main(int argc, char** argv) {
	TGAImage image(100, 100, TGAImage::RGB);
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	
	for (int i = 0; i < 100000; i++ ) {
		line(0, 1, 10, 10, image, red);
		line(13, 20, 80, 40, image, white); 
    	line(20, 13, 40, 80, image, red); 
    	line(80, 40, 13, 20, image, red);
	}
	
	image.write_tga_file("output2.tga");
	return 0;
}

