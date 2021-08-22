add_executable(server main.cpp)
target_link_libraries(server
  PRIVATE
    study_compile_options
    study_dependencies
    study_include_directories)
