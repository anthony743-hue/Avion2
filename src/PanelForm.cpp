#include "PanelForm.h"


PanelForm::PanelForm() :
    m_altitude("Altitude (m)", InputType::Number)
    , m_vx("Vitesse Vx (km/h)", InputType::Number)
    , m_vy("Vitesse Vy (km/h)", InputType::Number)
    , m_accMinX("Accélération min X (m/s²)", InputType::Number)
    , m_accMinY("Accélération min Y (m/s²)", InputType::Number)
    , m_accMinZ("Accélération min Z (m/s²)", InputType::Number)
    , m_vitesseDecrochage("Vitesse décrochage (km/h)", InputType::Number)
    , m_distanceRestante("Distance restante (m)", InputType::Number)
{
    m_altitude.setValue("500");
    m_vx.setValue("200");
    m_vy.setValue("0");
    m_accMinX.setValue("-2");
    m_accMinY.setValue("-2");
    m_accMinZ.setValue("-2");
    m_vitesseDecrochage.setValue("50");
    m_distanceRestante.setValue("3000");
}

void PanelForm::validate() const {
    m_altitude.validate();
    m_vx.validate();
    m_vy.validate();
    m_accMinX.validate();
    m_accMinY.validate();
    m_accMinZ.validate();
    m_vitesseDecrochage.validate();
    m_distanceRestante.validate();
}