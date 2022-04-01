set (shaders_source_files
IShader.h
DepthShader.cpp
DepthShader.h
GouraudShader.cpp
GouraudShader.h
NormalMappingShader.cpp
NormalMappingShader.h
PhongShader.cpp
PhongShader.h
TexturedShader.cpp
TexturedShader.h
)
add_library(shaders ${shaders_source_files})
target_link_libraries(shaders math)