#pragma once
#include <stdexcept>
#include <string>
#include "Vecteur.h"


class Avion
{
public:
    Avion();

    void setPosition(const Vecteur &pos);

    void setVitesse(const Vecteur &vitesse);

    void setAcceleration(const Vecteur &acceleration);

    void setAccelerationMin(const Vecteur &accMin);

    void setVitesseDecrochage(float vitesse);

    void setDistanceRestante(float distance);

    Vecteur getPosition() const { return m_position; }
    Vecteur getVitesse() const { return m_vitesse * 3.6f; } // retour en km/h
    Vecteur getVitesseMS() const { return m_vitesse; }      // retour en m/s
    Vecteur getAcceleration() const { return m_acceleration; }
    Vecteur getAccelerationMin() const { return m_accelerationMin; }
    float getVitesseDecrochage() const { return m_vitesseDecrochage * 3.6f; }
    float getDistanceRestante() const { return m_distanceRestante; }

    // Vitesse scalaire courante (norme) en km/h
    float getVitesseKmh() const { return m_vitesse.norme() * 3.6f; }
    // Vrai si la vitesse est passée sous la vitesse de décrochage
    bool estEnDecrochage() const { return m_enDecrochage; }
    // Vrai si l'avion a atteint/dépassé la piste (distance restante nulle)
    bool aAtterri() const { return m_distanceRestante <= EPSILON; }

    void update(float dt);
private:
    Vecteur m_position;             // mètres
    Vecteur m_vitesse;              // m/s
    Vecteur m_acceleration;         // m/s²
    Vecteur m_accelerationMin;      // m/s² (limite inférieure, typiquement négative)
    float m_vitesseDecrochage;    // m/s
    float m_distanceRestante;     // mètres
    bool m_enDecrochage = false;

    static constexpr float EPSILON = 1e-5f;
};