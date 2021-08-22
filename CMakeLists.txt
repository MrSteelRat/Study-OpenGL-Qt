cmake_minimum_required(VERSION 3.18)

project(study CXX)

set(CMAKE_CONFIGURATION_TYPES Debug Release)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_library(study_compile_options INTERFACE)
target_compile_features(study_compile_options INTERFACE cxx_std_17)

add_library(study_dependencies INTERFACE)

find_package(Qt6 COMPONENTS Widgets REQUIRED)
target_link_libraries(study_dependencies INTERFACE Qt6::Widgets)

add_library(study_include_directories INTERFACE)
target_include_directories(study_include_directories INTERFACE src)

#[[add_library(study_include_directories INTERFACE)
target_include_directories(study_include_directories 
  INTERFACE 
    deps/freeglut-3.2.1/include
    src)]]

add_subdirectory(deps)

add_subdirectory(src)
