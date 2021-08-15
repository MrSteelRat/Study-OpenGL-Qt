cmake_minimum_required(VERSION 3.10)

project(study CXX) 

add_library(study_compile_options INTERFACE) 
target_compile_features(study_compile_options INTERFACE cxx_std_17)

add_library(study_dependencies INTERFACE)
target_link_libraries(study_dependencies INTERFACE freeglut_static)

add_library(study_include_directories INTERFACE)
target_include_directories(study_include_directories 
  INTERFACE 
    deps/freeglut-3.2.1/include
    src)

add_subdirectory(deps)

add_subdirectory(src)
