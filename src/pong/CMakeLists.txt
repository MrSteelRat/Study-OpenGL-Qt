add_executable(pong main.cpp)
target_link_libraries(app 
  PRIVATE 
    study_compile_options
    study_dependencies
    study_include_directories)