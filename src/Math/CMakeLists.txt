set(
math_source_files
Vec.h
Matrix.h
)
add_library(math ${math_source_files})
set_target_properties(math PROPERTIES LINKER_LANGUAGE CXX)
