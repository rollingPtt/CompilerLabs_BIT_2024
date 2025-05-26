cmake_minimum_required(VERSION 3.16)
project(lab02)
set(CMAKE_CXX_STANDARD 14)
add_compile_options(-pedantic)
# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)
add_executable(Compilerlab2
  compilerlab2.cpp
)
target_compile_features(Compilerlab2 PRIVATE cxx_std_14)
