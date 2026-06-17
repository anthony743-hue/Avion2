#include "GlobalPanel.h"
#include "FileManager.h"
#include <QFileDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <algorithm>
#include <cmath>
#include <exception>

GlobalPanel::GlobalPanel(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Simulation de déplacement d'un avion (2D)");

    m_form = new PanelForm(this);
    m_view = new SimulationView(this);
    m_timer = new TimerPanel(this);
    m_statut = new QLabel("Prêt. Saisissez les données puis cliquez sur « Lancer ».", this);
    m_statut->setWordWrap(true);

    m_btnLancer = new QPushButton("Lancer", this);
    m_btnStop = new QPushButton("Stop", this);
    m_btnReprendre = new QPushButton("Reprendre", this);
    m_btnReset = new QPushButton("Réinitialiser", this);
    m_btnSauver = new QPushButton("Sauvegarder…", this);
    m_btnCharger = new QPushButton("Charger…", this);

    m_btnVxPlus = new QPushButton("+ Vx (γx)", this);
    m_btnVxMoins = new QPushButton("− Vx (γx min)", this);
    m_btnVyPlus = new QPushButton("+ Vy (γy)", this);
    m_btnVyMoins = new QPushButton("− Vy (γy min)", this);
    m_btnVxPlus->setToolTip("Augmente Vx du pas |Accélération X|");
    m_btnVxMoins->setToolTip("Réduit Vx du pas |Accélération min X|");
    m_btnVyPlus->setToolTip("Augmente Vy du pas |Accélération Y|");
    m_btnVyMoins->setToolTip("Réduit Vy du pas |Accélération min Y|");

    m_horloge = new QTimer(this);
    m_horloge->setInterval(kIntervalleMs);

    // Colonne de gauche : formulaire + fichiers.
    auto *fichiers = new QHBoxLayout;
    fichiers->addWidget(m_btnSauver);
    fichiers->addWidget(m_btnCharger);

    auto *colonneGauche = new QVBoxLayout;
    colonneGauche->addWidget(m_form);
    colonneGauche->addLayout(fichiers);
    colonneGauche->addStretch();

    // Groupe « Simulation » : Lancer / Stop / Reprendre / Réinitialiser.
    auto *grilleControles = new QGridLayout;
    grilleControles->addWidget(m_btnLancer, 0, 0);
    grilleControles->addWidget(m_btnStop, 0, 1);
    grilleControles->addWidget(m_btnReprendre, 1, 0);
    grilleControles->addWidget(m_btnReset, 1, 1);
    auto *groupeControles = new QGroupBox("Simulation", this);
    groupeControles->setLayout(grilleControles);

    // Groupe « Pilotage » : ajuste Vx / Vy en direct (2×2).
    auto *grillePilotage = new QGridLayout;
    grillePilotage->addWidget(m_btnVxPlus, 0, 0);
    grillePilotage->addWidget(m_btnVyPlus, 0, 1);
    grillePilotage->addWidget(m_btnVxMoins, 1, 0);
    grillePilotage->addWidget(m_btnVyMoins, 1, 1);
    auto *groupePilotage = new QGroupBox("Pilotage (vitesse)", this);
    groupePilotage->setLayout(grillePilotage);

    // Colonne de droite : vue, puis (timer + simulation + pilotage), statut.
    auto *colonneDroite = new QVBoxLayout;
    colonneDroite->addWidget(m_view, 1);
    auto *ligneBas = new QHBoxLayout;
    ligneBas->addWidget(m_timer);
    ligneBas->addWidget(groupeControles);
    ligneBas->addWidget(groupePilotage);
    colonneDroite->addLayout(ligneBas);
    colonneDroite->addWidget(m_statut);

    auto *principal = new QHBoxLayout(this);
    principal->addLayout(colonneGauche);
    principal->addLayout(colonneDroite, 1);

    m_view->setAvion(&m_avion);
    m_view->setTrajectoire(&m_trajectoire);

    connect(m_btnLancer, &QPushButton::clicked, this, &GlobalPanel::lancer);
    connect(m_btnStop, &QPushButton::clicked, this, &GlobalPanel::stop);
    connect(m_btnReprendre, &QPushButton::clicked, this, &GlobalPanel::reprendre);
    connect(m_btnReset, &QPushButton::clicked, this, &GlobalPanel::reinitialiser);
    connect(m_btnSauver, &QPushButton::clicked, this, &GlobalPanel::sauvegarder);
    connect(m_btnCharger, &QPushButton::clicked, this, &GlobalPanel::charger);
    connect(m_btnVxPlus, &QPushButton::clicked, this, [this] { ajusterVitesse(true, true); });
    connect(m_btnVxMoins, &QPushButton::clicked, this, [this] { ajusterVitesse(true, false); });
    connect(m_btnVyPlus, &QPushButton::clicked, this, [this] { ajusterVitesse(false, true); });
    connect(m_btnVyMoins, &QPushButton::clicked, this, [this] { ajusterVitesse(false, false); });
    connect(m_horloge, &QTimer::timeout, this, &GlobalPanel::tick);

    appliquerEtat(Etat::Vide);
}

void GlobalPanel::appliquerEtat(Etat etat) {
    m_etat = etat;
    const bool enCours = (etat == Etat::EnCours);
    const bool enPause = (etat == Etat::EnPause);

    m_btnLancer->setEnabled(!enCours);
    m_btnStop->setEnabled(enCours);
    m_btnReprendre->setEnabled(enPause);
    m_btnReset->setEnabled(etat != Etat::Vide);
    m_form->setEditable(!enCours && !enPause);
    m_btnCharger->setEnabled(!enCours && !enPause);

    // Le pilotage n'a de sens qu'avec une simulation active (en cours ou en pause).
    const bool pilotable = enCours || enPause;
    m_btnVxPlus->setEnabled(pilotable);
    m_btnVxMoins->setEnabled(pilotable);
    m_btnVyPlus->setEnabled(pilotable);
    m_btnVyMoins->setEnabled(pilotable);
}

void GlobalPanel::ajusterVitesse(bool axeX, bool augmenter) {
    if (m_etat != Etat::EnCours && m_etat != Etat::EnPause)
        return;

    Vecteur v = m_avion.getVitesseMS();             // m/s
    const Vecteur acc = m_avion.getAcceleration();      // pas pour augmenter (m/s²)
    const Vecteur accMin = m_avion.getAccelerationMin(); // pas pour réduire (m/s²)

    if (axeX)
        v.x += augmenter ? std::fabs(acc.x) : -std::fabs(accMin.x);
    else
        v.y += augmenter ? std::fabs(acc.y) : -std::fabs(accMin.y);

    // L'avion ne recule pas : on borne à 0 comme dans update().
    v.x = std::max(0.f, v.x);
    v.y = std::max(0.f, v.y);
    m_avion.setVitesse(v);
    m_view->update();

    m_statut->setText(QString("Vitesse ajustée : Vx = %1 km/h, Vy = %2 km/h.")
                          .arg(v.x * 3.6f, 0, 'f', 0)
                          .arg(v.y * 3.6f, 0, 'f', 0));
}

void GlobalPanel::lancer() {
    try {
        m_avion = m_form->construireAvion();
    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Données invalides", e.what());
        return;
    }

    m_trajectoire.clear();
    m_trajectoire.push_back(m_avion.getPosition());
    m_tempsEcoule = 0.0;
    m_timer->reset();
    m_view->setDistanceInitiale(m_avion.getDistanceRestante());
    m_view->update();

    m_statut->setText("Simulation en cours…");
    appliquerEtat(Etat::EnCours);
    m_horloge->start();
}

void GlobalPanel::stop() {
    if (m_etat != Etat::EnCours)
        return;
    m_horloge->stop();  // arrête la simulation ET le timer (plus de tick)
    m_statut->setText("Simulation en pause. Cliquez sur « Reprendre ».");
    appliquerEtat(Etat::EnPause);
}

void GlobalPanel::reprendre() {
    if (m_etat != Etat::EnPause)
        return;
    // L'avion conserve son état : la simulation reprend depuis la dernière position.
    m_statut->setText("Simulation en cours…");
    appliquerEtat(Etat::EnCours);
    m_horloge->start();
}

void GlobalPanel::reinitialiser() {
    m_horloge->stop();
    m_trajectoire.clear();
    m_avion = Avion();
    m_tempsEcoule = 0.0;
    m_timer->reset();
    m_view->setDistanceInitiale(0.f);
    m_view->update();
    m_statut->setText("Réinitialisé. Saisissez les données puis cliquez sur « Lancer ».");
    appliquerEtat(Etat::Vide);
}

void GlobalPanel::tick() {
    const float dt = (kIntervalleMs / 1000.0f) * kEchelleTemps;
    m_avion.update(dt);
    m_tempsEcoule += dt;
    m_trajectoire.push_back(m_avion.getPosition());
    m_timer->setElapsed(m_tempsEcoule);
    m_view->update();

    if (m_avion.aAtterri()) {
        m_horloge->stop();
        m_statut->setText(QString("Arrivée sur la piste atteinte en %1 s (temps simulé).")
                              .arg(m_tempsEcoule, 0, 'f', 1));
        appliquerEtat(Etat::Termine);
    } else if (m_avion.estEnDecrochage()) {
        m_statut->setText(QString("⚠ Décrochage : vitesse %1 km/h sous le seuil.")
                              .arg(m_avion.getVitesseKmh(), 0, 'f', 0));
    }
}

void GlobalPanel::sauvegarder() {
    Avion avion;
    try {
        avion = m_form->construireAvion();
    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Données invalides", e.what());
        return;
    }
    const QString chemin = QFileDialog::getSaveFileName(
        this, "Sauvegarder les paramètres", "avion.txt", "Fichiers texte (*.txt)");
    if (chemin.isEmpty())
        return;
    try {
        FileManager::sauvegarderParametres(chemin.toStdString(), avion);
        m_statut->setText("Paramètres sauvegardés dans " + chemin);
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Erreur de sauvegarde", e.what());
    }
}

void GlobalPanel::charger() {
    const QString chemin = QFileDialog::getOpenFileName(
        this, "Charger les paramètres", QString(), "Fichiers texte (*.txt)");
    if (chemin.isEmpty())
        return;
    try {
        const Avion avion = FileManager::chargerParametres(chemin.toStdString());
        m_form->remplirDepuis(avion);
        m_statut->setText("Paramètres chargés depuis " + chemin);
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Erreur de chargement", e.what());
    }
}
