cmake_minimum_required(VERSION 3.16)   # Version minimale de CMake
project(Avion LANGUAGES CXX)           # Nom du projet, langage C++

set(CMAKE_CXX_STANDARD 23)             # Utiliser C++23
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Traitement automatique des macros Qt (Q_OBJECT, signaux/slots, ressources, UI).
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)  # Trouver Qt6 (Widgets)

add_executable(avion_sim
    main.cpp
    src/Avion.cpp
    src/InputField.cpp
    src/PanelForm.cpp
    src/FileManager.cpp
    src/TimerPanel.cpp
    src/SimulationView.cpp
    src/GlobalPanel.cpp
    # En-têtes listés pour qu'AUTOMOC traite les classes Q_OBJECT.
    include/Avion.h
    include/Vecteur.h
    include/InputField.h
    include/PanelForm.h
    include/FileManager.h
    include/TimerPanel.h
    include/SimulationView.h
    include/GlobalPanel.h
)

target_include_directories(avion_sim PRIVATE include)
target_link_libraries(avion_sim PRIVATE Qt6::Widgets)
