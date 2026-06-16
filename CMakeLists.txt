cmake_minimum_required(VERSION 3.16)   # Version minimale de CMake
project(Avion LANGUAGES CXX)   # Nom du projet, langage C++

set(CMAKE_CXX_STANDARD 23)             # Utiliser C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)  # Trouver Qt6 (Widgets)
qt_standard_project_setup()                    # Configurer Qt

add_executable(avion_sim main.cpp)               # Créer l'exécutable à partir de main.cpp
target_link_libraries(avion_sim Qt6::Widgets)    # Lier avec les bibliothèques Qt Widgets