#pragma once
#include "InputField.h"
#include "Vecteur.h"
#include <stdexcept>
#include <string>

class PanelForm {
public :
    PanelForm();

    InputField& fieldAltitude()        { return m_altitude; }
    InputField& fieldVx()              { return m_vx; }
    InputField& fieldVy()              { return m_vy; }
    InputField& fieldAccMinX()         { return m_accMinX; }
    InputField& fieldAccMinY()         { return m_accMinY; }
    InputField& fieldAccMinZ()         { return m_accMinZ; }
    InputField& fieldVitesseDecrochage() { return m_vitesseDecrochage; }
    InputField& fieldDistanceRestante() { return m_distanceRestante; }

    void validate() const;
private:
    InputField m_altitude;
    InputField m_vx;
    InputField m_vy;
    InputField m_accMinX;
    InputField m_accMinY;
    InputField m_accMinZ;
    InputField m_vitesseDecrochage;
    InputField m_distanceRestante;
};