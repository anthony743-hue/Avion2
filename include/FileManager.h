#pragma once
#include "Avion.h"
#include "Vecteur.h"
#include <string>
#include <vector>

// FileManager : regroupe toutes les méthodes qui touchent aux fichiers
// (sauvegarde / chargement des paramètres de l'avion et de sa trajectoire).
class FileManager {
public:
    // Sauvegarde les paramètres de l'avion dans un fichier texte (clé=valeur).
    // Lève std::runtime_error si le fichier ne peut pas être ouvert.
    static void sauvegarderParametres(const std::string &chemin, const Avion &avion);

    // Recharge un avion depuis un fichier produit par sauvegarderParametres.
    static Avion chargerParametres(const std::string &chemin);

    // Enregistre la trajectoire (suite de positions, en mètres) au format CSV.
    static void sauvegarderTrajectoire(const std::string &chemin,
                                       const std::vector<Vecteur> &points);
};
