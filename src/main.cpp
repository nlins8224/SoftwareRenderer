#include <vector>
#include <iostream>

#include "tgaimage.h"
#include "model.h"
#include "Math/Vec.h"
#include "Math/Matrix.h"
#include "gl.h"
#include "Shaders/PhongShader.h"
#include "Shaders/DepthShader.h"

#define SHADOWS_ENABLED 1

const int WIDTH = 800;
const int HEIGHT = 800;
const int DEPTH = 255;

Vec3f  lightDir(1, 1, 1);
Vec3f       eye(1, 1, 3);
Vec3f    center(0, 0, 0);
Vec3f        up(0, 1, 0);


void render_shadow(Model& model, int width, int height, int depth) {
    TGAImage depth_img(width, height, TGAImage::RGB);
    TGAImage shadowbuffer(width, height, TGAImage::GRAYSCALE);
    lookat(lightDir, center, up);
    viewport(width / 8, height / 8, width * 3/4, height * 3/4, depth);
    projection(0);
    
    DepthShader depth_shader{model, depth};
    Vec4f screen_coords[3];
    for (int i = 0; i < model.nfaces(); i++) {
        for (int j = 0; j < 3; j++) {
            screen_coords[j] = depth_shader.vertex(i, j);
        }
        triangle(screen_coords, depth_shader, depth_img, shadowbuffer);
    }
    depth_img.flip_vertically(); 
    depth_img.write_tga_file("depth.tga");

}

void render(Model& model, int width, int height, int depth) {

    lightDir = lightDir.normalized();

	TGAImage image  (width, height, TGAImage::RGB);
    TGAImage zbuffer(width, height, TGAImage::GRAYSCALE);

    #if SHADOWS_ENABLED
        render_shadow(model, width, height, depth);
    #endif

    lookat(eye, center, up);
    viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4, depth);
    projection(-1.f/(eye - center).norm());
    
    PhongShader shader{ model, lightDir };
	for (int i = 0; i < model.nfaces(); i++) {
        Vec4f screenCoords[3];

        for (int j = 0; j < 3; j++) {
			screenCoords[j] = shader.vertex(i, j);
		}
		triangle(screenCoords, shader, image, zbuffer);
	}

	/* place the origin in the bottom left corner of the image */
	image.  flip_vertically();
    zbuffer.flip_vertically();
    image.  write_tga_file("output.tga");
    zbuffer.write_tga_file("zbuffer.tga");

}

int main() {
    //TODO: smart pointers
    int *zbuffer = NULL;
    // TODO: check if file exists, ptr could be null
	Model* ptr_model = new Model("../obj/african_head.obj");
    Model& model { *ptr_model };
	render(model, WIDTH, HEIGHT, DEPTH);
    delete ptr_model;
    delete zbuffer;
	return 0;
}

