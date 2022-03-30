#include <vector>
#include <iostream>

#include "tgaimage.h"
#include "model.h"
#include "Vec.h"
#include "Matrix.h"
#include "gl.h"
#include "NormalMappingShader.h"

const int WIDTH = 800;
const int HEIGHT = 800;
const int DEPTH = 255;

//TODO: smart pointers
int *zbuffer = NULL;

Vec3f  lightDir(1, 1, 1);
Vec3f       eye(1, 1, 3);
Vec3f    center(0, 0, 0);
Vec3f        up(0, 1, 0);

/*
TODO: move matrices to prepareMatrices function
 */
void render(Model& model, int width, int height, int depth) {

	lookat(eye, center, up);
    viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4, depth);
    projection(-1.f/(eye - center).norm());
    lightDir = lightDir.normalized();

	TGAImage image  (width, height, TGAImage::RGB);
    TGAImage zbuffer(width, height, TGAImage::GRAYSCALE);

	// std::cerr << ModelView << std::endl;
    // std::cerr << Projection << std::endl;
    // std::cerr << Viewport << std::endl;
    // Matrix transformed = (Viewport * Projection * ModelView);
    // std::cerr << transformed << std::endl;

    NormalMappingShader shader{ model, lightDir };
    shader.set_uniform_PMV(Projection * ModelView);
    shader.set_uniform_PMVIT((Projection * ModelView).invert_transpose());
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
    // TODO: check if file exists, ptr could be null
	Model* ptr_model = new Model("obj/african_head.obj");
    // TODO: Model class should have reference constructor?
    Model& model { *ptr_model };
	render(model, WIDTH, HEIGHT, DEPTH);
    delete ptr_model;
	return 0;
}

