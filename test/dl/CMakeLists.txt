cmake_minimum_required(VERSION 2.8)

file(files_com ../src/com/*.cpp ../src/com/*.h)
set(src_app ./main.cpp ${files_com})

project(com)
include_directories("D:/linux_boost_1_71_0/include")
link_directories("D:/linux_boost_1_71_0/lib")
add_executable(${PROJECT_NAME} ${src_app})
target_link_libraries (${PROJECT_NAME} boost_system boost_coroutine boost_context)
target_link_libraries (${PROJECT_NAME} pthread)