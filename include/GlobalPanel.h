#pragma once
#include "Avion.h"
#include "PanelForm.h"
#include "SimulationView.h"
#include "TimerPanel.h"
#include "Vecteur.h"
#include <QWidget>
#include <vector>

class QPushButton;
class QLabel;
class QTimer;

// Panel global : assemble le formulaire, la vue 2D, le timer et les
// commandes (Lancer / Stop / Reprendre / Réinitialiser / fichiers).
class GlobalPanel : public QWidget {
    Q_OBJECT
public:
    explicit GlobalPanel(QWidget *parent = nullptr);

private slots:
    void lancer();        // démarre une nouvelle simulation
    void stop();          // met en pause la simulation et le timer
    void reprendre();     // reprend depuis la dernière position
    void reinitialiser(); // remet à zéro
    void sauvegarder();   // sauvegarde les paramètres dans un fichier
    void charger();       // recharge les paramètres depuis un fichier
    void tick();          // un pas de simulation

private:
    enum class Etat { Vide, EnCours, EnPause, Termine };
    void appliquerEtat(Etat etat);
    // Ajuste une composante de la vitesse d'un pas : le pas vient de
    // l'accélération (augmenter) ou de l'accélération min (réduire).
    void ajusterVitesse(bool axeX, bool augmenter);

    PanelForm *m_form;
    SimulationView *m_view;
    TimerPanel *m_timer;
    QLabel *m_statut;

    QPushButton *m_btnLancer;
    QPushButton *m_btnStop;
    QPushButton *m_btnReprendre;
    QPushButton *m_btnReset;
    QPushButton *m_btnSauver;
    QPushButton *m_btnCharger;

    // Pilotage : ajustent Vx / Vy en cours de simulation.
    QPushButton *m_btnVxPlus;
    QPushButton *m_btnVxMoins;
    QPushButton *m_btnVyPlus;
    QPushButton *m_btnVyMoins;

    QTimer *m_horloge;
    Avion m_avion;
    std::vector<Vecteur> m_trajectoire;
    double m_tempsEcoule = 0.0;  // secondes simulées
    Etat m_etat = Etat::Vide;

    static constexpr int kIntervalleMs = 50;     // période du timer (ms)
    static constexpr float kEchelleTemps = 4.0f; // facteur d'accélération de la simulation
};
