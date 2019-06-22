cmake_minimum_required(VERSION 2.8)

project(windmill)
set(CMAKE_CXX_STANDARD 14)
add_compile_options(
    -o3
)

add_library(ArmorTrigger ArmorTrigger.cpp)
target_link_libraries(ArmorTrigger ${OpenCV_LIBS} )
