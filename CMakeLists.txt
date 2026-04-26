cmake_minimum_required(VERSION 3.16)
project(Pacman)

set(CMAKE_CXX_STANDARD 17)

# SFML in kurulu olduğu yol
set(SFML_DIR "C:/SFML2/SFML-2.6.2/lib/cmake/SFML")

# SFML kütüphanelerini bulmak icin
find_package(SFML 2.6 COMPONENTS graphics window system audio REQUIRED)

# Kaynak dosyaları
add_executable(Pacman src/main.cpp)

# SFML i projeye bağlar
target_link_libraries(Pacman sfml-graphics sfml-window sfml-system sfml-audio)