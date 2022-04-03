# Software Renderer

This project is a small software renderer which pipeline mimicks that of OpenGL in a simplified version. It is based on a [short computer graphics course](https://github.com/ssloy/tinyrenderer/wiki/Lesson-0:-getting-started) written by Dmitry Sokolov. It is written in C++17 and has no dependencies (especially no graphic libraries since the goal was to write one).

## Compilation:

```
git clone https://github.com/nlins8224/SoftwareRenderer.git &&
mkdir build &&
cd build &&
cmake ../src &&
make && 
./renderer ../obj/head.obj
```

The rendered image is saved to `output.tga` in `build`

## Some Exmaple Images:

![PhongShader](https://i.ibb.co/4MrtdQ0/Screenshot-from-2022-04-03-19-14-20.png)

![GouraudShader](https://i.ibb.co/R2BQcg3/Screenshot-from-2022-04-03-19-08-57.png)

![DepthShader](https://i.ibb.co/d2Z1vM2/Screenshot-from-2022-04-02-22-29-47.png)

![TexturedShader](https://i.ibb.co/3BP2zxK/Screenshot-from-2022-04-02-22-05-38.png)

![TexturedShader](https://i.ibb.co/zHKhgpX/Screenshot-from-2022-04-03-19-11-42.png)

![NormalMappingShader](https://i.ibb.co/270fccv/Screenshot-from-2022-04-03-19-13-09.png)

