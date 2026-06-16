#include <QApplication>
#include <QWidget>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);   // 1. Initialise l'application

    QWidget fenetre;               // 2. Crée une fenêtre
    fenetre.resize(400, 300);
    fenetre.show();

    return app.exec();             // 3. Démarre la boucle événementielle
}