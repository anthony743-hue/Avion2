#include "PanelForm.h"
#include <QFormLayout>

PanelForm::PanelForm(QWidget *parent) :
    QGroupBox("Données initiales", parent),
    m_altitude(new InputField("Altitude (m)", InputType::Number, this)),
    m_vx(new InputField("Vitesse Vx (km/h)", InputType::Number, this)),
    m_vy(new InputField("Vitesse Vy (km/h)", InputType::Number, this)),
    m_ax(new InputField("Accélération Ax (m/s²)", InputType::Number, this)),
    m_ay(new InputField("Accélération Ay (m/s²)", InputType::Number, this)),
    m_accMinX(new InputField("Accélération min X (m/s²)", InputType::Number, this)),
    m_accMinY(new InputField("Accélération min Y (m/s²)", InputType::Number, this)),
    m_vitesseDecrochage(new InputField("Vitesse décrochage (km/h)", InputType::Number, this)),
    m_distanceRestante(new InputField("Distance restante (m)", InputType::Number, this))
{
    // Valeurs par défaut (la position X vaut 0 par défaut, cf. todo).
    m_altitude->setValue("500");
    m_vx->setValue("250");
    m_vy->setValue("0");
    m_ax->setValue("0");
    m_ay->setValue("0");
    m_accMinX->setValue("-5");
    m_accMinY->setValue("-5");
    m_vitesseDecrochage->setValue("150");
    m_distanceRestante->setValue("3000");

    auto *layout = new QFormLayout(this);
    layout->addRow("Altitude (m)", m_altitude);
    layout->addRow("Vitesse Vx (km/h)", m_vx);
    layout->addRow("Vitesse Vy (km/h)", m_vy);
    layout->addRow("Accélération Ax (m/s²)", m_ax);
    layout->addRow("Accélération Ay (m/s²)", m_ay);
    layout->addRow("Accélération min X (m/s²)", m_accMinX);
    layout->addRow("Accélération min Y (m/s²)", m_accMinY);
    layout->addRow("Vitesse décrochage (km/h)", m_vitesseDecrochage);
    layout->addRow("Distance restante (m)", m_distanceRestante);
}

void PanelForm::validate() const {
    m_altitude->validate();
    m_vx->validate();
    m_vy->validate();
    m_ax->validate();
    m_ay->validate();
    m_accMinX->validate();
    m_accMinY->validate();
    m_vitesseDecrochage->validate();
    m_distanceRestante->validate();
}

Avion PanelForm::construireAvion() const {
    validate();

    Avion avion;
    // Position : X = 0 par défaut, Y = altitude.
    avion.setPosition({0.f, m_altitude->getFloat()});

    // L'accélération min doit être définie avant l'accélération (règle de gestion).
    avion.setAccelerationMin({m_accMinX->getFloat(), m_accMinY->getFloat()});

    // Vitesse saisie en km/h -> convertie en m/s pour le modèle.
    const float vx = m_vx->getFloat() / 3.6f;
    const float vy = m_vy->getFloat() / 3.6f;
    avion.setVitesse({vx, vy});

    avion.setAcceleration({m_ax->getFloat(), m_ay->getFloat()});
    avion.setVitesseDecrochage(m_vitesseDecrochage->getFloat());
    avion.setDistanceRestante(m_distanceRestante->getFloat());
    return avion;
}

void PanelForm::remplirDepuis(const Avion &avion) {
    const Vecteur pos = avion.getPosition();
    const Vecteur vit = avion.getVitesse();          // km/h
    const Vecteur acc = avion.getAcceleration();     // m/s²
    const Vecteur accMin = avion.getAccelerationMin();

    m_altitude->setValue(std::to_string(pos.y));
    m_vx->setValue(std::to_string(vit.x));
    m_vy->setValue(std::to_string(vit.y));
    m_ax->setValue(std::to_string(acc.x));
    m_ay->setValue(std::to_string(acc.y));
    m_accMinX->setValue(std::to_string(accMin.x));
    m_accMinY->setValue(std::to_string(accMin.y));
    m_vitesseDecrochage->setValue(std::to_string(avion.getVitesseDecrochage()));
    m_distanceRestante->setValue(std::to_string(avion.getDistanceRestante()));
}

void PanelForm::setEditable(bool editable) {
    m_altitude->setReadOnly(!editable);
    m_vx->setReadOnly(!editable);
    m_vy->setReadOnly(!editable);
    m_ax->setReadOnly(!editable);
    m_ay->setReadOnly(!editable);
    m_accMinX->setReadOnly(!editable);
    m_accMinY->setReadOnly(!editable);
    m_vitesseDecrochage->setReadOnly(!editable);
    m_distanceRestante->setReadOnly(!editable);
}
