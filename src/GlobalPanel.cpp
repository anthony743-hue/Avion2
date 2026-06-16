#include "GlobalPanel.h"
#include "FileManager.h"
#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
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

    // Boutons de contrôle.
    auto *controles = new QHBoxLayout;
    controles->addWidget(m_btnLancer);
    controles->addWidget(m_btnStop);
    controles->addWidget(m_btnReprendre);
    controles->addWidget(m_btnReset);

    // Colonne de droite : vue, timer, contrôles, statut.
    auto *colonneDroite = new QVBoxLayout;
    colonneDroite->addWidget(m_view, 1);
    auto *ligneTimer = new QHBoxLayout;
    ligneTimer->addWidget(m_timer);
    ligneTimer->addLayout(controles, 1);
    colonneDroite->addLayout(ligneTimer);
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
