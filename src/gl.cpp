#include <cmath>
#include <limits>
#include <cstdlib>
#include "gl.h"

Matrix4f ModelView;
Matrix4f Viewport;
Matrix4f Projection;

void viewport(int x, int y, int w, int h, int d) {
	Viewport = Matrix4f::identity();
	// Scale
	Viewport[0][0] = w / 2.f;
	Viewport[1][1] = h / 2.f;
	Viewport[2][2] = d / 2.f;
	// Translate
	Viewport[0][3] = x + w / 2.f;
	Viewport[1][3] = y + h / 2.f;
	Viewport[2][3] = d / 2.f;
}

void projection(float coeff) {
	Projection = Matrix4f::identity();
    Projection[3][2] = coeff;
}

void lookat(Vec3f eye, Vec3f center, Vec3f up) {
	Vec3f z = (eye - center).normalized();
	Vec3f x = cross_prod(up, z).normalized();
	Vec3f y = cross_prod(z, x).normalized();
	ModelView = Matrix4f::identity();
    for (int i = 0; i < 3; i++) {
        ModelView[0][i] = x[i];
        ModelView[1][i] = y[i];
        ModelView[2][i] = z[i];
        ModelView[i][3] = -center[i];
    }
}

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
Vec3f barycentric(Vec2f A, Vec2f B, Vec2f C, Vec2f P) {
	
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

	Vec3f u = cross_prod(V1, V2);

	// degenerate case
	if (std::abs(u.z) < 1) return Vec3f(-1, 1, 1);
	
	// (1 - u - v, u, v)
	return Vec3f(1.f - (u.x + u.y) / u.z, 
	             u.y / u.z, 
				 u.x / u.z);
}

/* Pixel is inside triangle if barycentric coordinates parameters 0 <= u, v, z <= 1 */
bool in_triangle(Vec3f barycentric) {
	return barycentric.x >= 0 && barycentric.y >= 0 && barycentric.z >= 0;
}

bool is_visible(int depth, int frag_depth) {
	return depth < frag_depth;
}

int calculate_frag_depth(Vec4f *pts, Vec3f barycentric) {
 	float z = pts[0][2] * barycentric.x + pts[1][2] * barycentric.y + pts[2][2] * barycentric.z;
 	float w = pts[0][3] * barycentric.x + pts[1][3] * barycentric.y + pts[2][3] * barycentric.z;
 	return std::max(0, std::min(255, int(z / w + .5)));
 }

/* 
For each pixel in bounding box if pixel is inside triangle then color that pixel
*/
//TODO: *pts -> ...args
void triangle(Vec4f *pts, IShader &shader, TGAImage &image, TGAImage &zbuffer) {
 	Vec2f bounding_box_max(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
 	Vec2f bounding_box_min(std::numeric_limits<float>::max(),  std::numeric_limits<float>::max());

    /* set bounding box */
 	for (int i = 0; i < 3; i++) {
 		for (int j = 0; j < 2; j++) {
 			bounding_box_min[j] = std::min(bounding_box_min[j], pts[i][j] / pts[i][3]);
		    bounding_box_max[j] = std::max(bounding_box_max[j], pts[i][j] / pts[i][3]);
 		}
 	}
 	Vec2i P;
    TGAColor color;
 	/* for each pixel in bounding box check if it is inside triangle and if should be rendered */
 	for (P.x = bounding_box_min.x; P.x <= bounding_box_max.x; P.x++) {
 		for (P.y = bounding_box_min.y; P.y <= bounding_box_max.y; P.y++) {
 			Vec3f bar = barycentric(proj<2>(pts[0] / pts[0][3]), 
			 						proj<2>(pts[1] / pts[1][3]), 
									proj<2>(pts[2] / pts[2][3]), 
									proj<2>(P));

 			int frag_depth = calculate_frag_depth(pts, bar);
			int depth = zbuffer.get(P.x, P.y)[0];
 			//TODO: Move this to separate function (is_visible)
 			if (!in_triangle(bar) || !(is_visible(depth, frag_depth))) continue;
 			bool discard = shader.fragment(bar, color);
 			if (!discard) {
 				zbuffer.set(P.x, P.y, TGAColor(frag_depth));
 				image.set(P.x, P.y, color);
 			}
 		}
 	}
 }
