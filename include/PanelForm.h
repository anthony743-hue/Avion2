#pragma once
#include "Avion.h"
#include "InputField.h"
#include <QGroupBox>

// Panel de saisie (formulaire) des données initiales de l'avion.
class PanelForm : public QGroupBox {
    Q_OBJECT
public:
    explicit PanelForm(QWidget *parent = nullptr);

    // Valide tous les champs (lève std::invalid_argument si une valeur est invalide).
    void validate() const;

    // Construit un Avion à partir des champs saisis (valide d'abord).
    Avion construireAvion() const;

    // Remplit les champs à partir d'un Avion existant.
    void remplirDepuis(const Avion &avion);

    // Active/désactive la saisie (verrouillé pendant la simulation).
    void setEditable(bool editable);

    InputField *fieldAltitude() const { return m_altitude; }
    InputField *fieldVx() const { return m_vx; }
    InputField *fieldVy() const { return m_vy; }
    InputField *fieldAx() const { return m_ax; }
    InputField *fieldAy() const { return m_ay; }
    InputField *fieldAccMinX() const { return m_accMinX; }
    InputField *fieldAccMinY() const { return m_accMinY; }
    InputField *fieldVitesseDecrochage() const { return m_vitesseDecrochage; }
    InputField *fieldDistanceRestante() const { return m_distanceRestante; }

private:
    InputField *m_altitude;
    InputField *m_vx;
    InputField *m_vy;
    InputField *m_ax;
    InputField *m_ay;
    InputField *m_accMinX;
    InputField *m_accMinY;
    InputField *m_vitesseDecrochage;
    InputField *m_distanceRestante;
};
