#!/bin/bash
set -e                       # Arrêt immédiat en cas d'erreur

BUILD_DIR="build"            # Dossier de compilation
EXECUTABLE="avion_sim"         # Nom de l'exécutable (adaptez si nécessaire)

# Étape 1 : créer le dossier de build et s'y rendre
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Étape 2 : configurer avec CMake (mode Release par défaut)
cmake .. -DCMAKE_BUILD_TYPE=Release

# Étape 3 : compiler en parallèle
make -j$(nproc)

# Revenir au dossier du projet
cd ..

# Étape 4 : lancer l'application
echo "Lancement de $BUILD_DIR/$EXECUTABLE ..."
"./$BUILD_DIR/$EXECUTABLE"