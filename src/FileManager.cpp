#include "FileManager.h"
#include <fstream>
#include <map>
#include <sstream>
#include <stdexcept>

using namespace std;

void FileManager::sauvegarderParametres(const string &chemin, const Avion &avion) {
    ofstream fichier(chemin);
    if (!fichier)
        throw runtime_error("Impossible d'ouvrir le fichier en écriture : " + chemin);

    const Vecteur pos = avion.getPosition();
    const Vecteur vitMS = avion.getVitesseMS();
    const Vecteur acc = avion.getAcceleration();
    const Vecteur accMin = avion.getAccelerationMin();

    fichier << "posX=" << pos.x << '\n'
            << "posY=" << pos.y << '\n'
            << "vitX=" << vitMS.x << '\n'      // m/s
            << "vitY=" << vitMS.y << '\n'      // m/s
            << "accX=" << acc.x << '\n'        // m/s²
            << "accY=" << acc.y << '\n'        // m/s²
            << "accMinX=" << accMin.x << '\n'  // m/s²
            << "accMinY=" << accMin.y << '\n'  // m/s²
            << "vDecrochageKmh=" << avion.getVitesseDecrochage() << '\n'  // km/h
            << "distanceRestante=" << avion.getDistanceRestante() << '\n';
}

Avion FileManager::chargerParametres(const string &chemin) {
    ifstream fichier(chemin);
    if (!fichier)
        throw runtime_error("Impossible d'ouvrir le fichier en lecture : " + chemin);

    map<string, float> valeurs;
    string ligne;
    while (getline(fichier, ligne)) {
        if (ligne.empty() || ligne[0] == '#')
            continue;
        const auto sep = ligne.find('=');
        if (sep == string::npos)
            continue;
        const string cle = ligne.substr(0, sep);
        try {
            valeurs[cle] = stof(ligne.substr(sep + 1));
        } catch (const exception &) {
            throw runtime_error("Valeur invalide pour la clé \"" + cle + "\" dans " + chemin);
        }
    }

    auto lire = [&](const string &cle) -> float {
        auto it = valeurs.find(cle);
        if (it == valeurs.end())
            throw runtime_error("Clé manquante \"" + cle + "\" dans " + chemin);
        return it->second;
    };

    Avion avion;
    avion.setPosition({lire("posX"), lire("posY")});
    // L'accélération min doit être définie avant l'accélération (règle de gestion).
    avion.setAccelerationMin({lire("accMinX"), lire("accMinY")});
    avion.setVitesse({lire("vitX"), lire("vitY")});  // m/s
    avion.setAcceleration({lire("accX"), lire("accY")});
    avion.setVitesseDecrochage(lire("vDecrochageKmh"));  // km/h
    avion.setDistanceRestante(lire("distanceRestante"));
    return avion;
}

void FileManager::sauvegarderTrajectoire(const string &chemin,
                                         const vector<Vecteur> &points) {
    ofstream fichier(chemin);
    if (!fichier)
        throw runtime_error("Impossible d'ouvrir le fichier en écriture : " + chemin);

    fichier << "x,y\n";
    for (const Vecteur &p : points)
        fichier << p.x << ',' << p.y << '\n';
}
