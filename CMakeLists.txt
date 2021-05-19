cmake_minimum_required(VERSION 3.19)
project(untitled)

set(CMAKE_CXX_STANDARD 14)


#add_executable(untitled main.cpp) is 
add_executable(untitled glad.c shader.h stb_image.cpp stb_image.h ball.cpp)
set(SOURCE_FILES glad.c shader.h stb_image.cpp stb_image.h ball.cpp)
target_link_libraries(untitled GL GLU glfw3 X11 Xxf86vm Xrandr pthread Xi dl Xinerama Xcursor assimp)

