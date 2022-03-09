#include <vector>
#include <iostream>

#include "tgaimage.h"
#include "model.h"
#include "geometry.h"
#include "gl.h"

const int WIDTH = 800;
const int HEIGHT = 800;
const int DEPTH = 255;

Model *model = NULL;
int *zbuffer = NULL;

Vec3f  lightDir(1, 1, 1);
Vec3f       eye(0, -1, 3);
Vec3f    center(0, 0, 0);
Vec3f        up(0, 1, 0);

/*TODO: This should be a class in separate file */
struct GouraudShader : public IShader {
    Vec3f varying_intensity; // written by vertex shader, read by fragment shader

    virtual Vec4f vertex(int iface, int nthvert) {
        Vec4f gl_Vertex = embed<4>(model->vert(iface, nthvert)); // read the vertex from .obj file
        gl_Vertex = Viewport * Projection * ModelView * gl_Vertex;     // transform it to screen coordinates
        varying_intensity[nthvert] = std::max(0.f, model->normal(iface, nthvert)*lightDir); // get diffuse lighting intensity
        return gl_Vertex;
    }

    virtual bool fragment(Vec3f bar, TGAColor &color) {
        float intensity = varying_intensity*bar;   // interpolate intensity for the current pixel
        color = TGAColor(255, 255, 255)*intensity; // well duh
        return false;                              // no, we do not discard this pixel
    }
};

/*
TODO: move matrices to prepareMatrices function
 */
void render(Model *model, int width, int height, int depth, float scale) {

	lookat(eye, center, up);
    viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4, depth);
    projection(-1.f/(eye - center).norm());
    lightDir.normalize();

	TGAImage image  (width, height, TGAImage::RGB);
    TGAImage zbuffer(width, height, TGAImage::GRAYSCALE);

	std::cerr << ModelView << std::endl;
    std::cerr << Projection << std::endl;
    std::cerr << Viewport << std::endl;
    Matrix transformed = (Viewport * Projection * ModelView);
    std::cerr << transformed << std::endl;

    GouraudShader shader;
	for (int i = 0; i < model->nfaces(); i++) {
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

	delete model;
}

void init() {
	model = new Model("obj/african_head.obj");
}


int main(int argc, char** argv) {
	init();
	float scale = 8;
	render(model, WIDTH, HEIGHT, DEPTH, scale);
	return 0;
}

