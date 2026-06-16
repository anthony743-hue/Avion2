#include "GlobalPanel.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    GlobalPanel fenetre;           // Panel global de la simulation
    fenetre.resize(1000, 560);
    fenetre.show();

    return app.exec();             // Démarre la boucle événementielle
}
