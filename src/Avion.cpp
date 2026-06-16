#include "Avion.h"
#include <iostream>
#include <algorithm>

using namespace std;

Avion::Avion(): 
    m_position{0.f, 0.f}
    , m_vitesse{0.f, 0.f}
    , m_acceleration{0.f, 0.f}
    , m_accelerationMin{-2.f, -2.f}
    , m_vitesseDecrochage{600.f / 3.6f}      // 600 km/h par défaut
    , m_distanceRestante{0.f}
{}

void Avion::setPosition(const Vecteur& pos){
    m_position = pos;
}

void Avion::setVitesse(const Vecteur& vitesse){
    m_vitesse = vitesse;
}

void Avion::setAcceleration(const Vecteur& acceleration) {
    if (acceleration.x < m_accelerationMin.x - EPSILON ||
        acceleration.y < m_accelerationMin.y - EPSILON ) {
        throw logic_error("Accélération inférieure au minimum autorisé (décrochage structurel).");
    }
    m_acceleration = acceleration;
}

void Avion::setVitesseDecrochage(float vitesse_kmh) {
    if (vitesse_kmh <= 0.f) {
        throw invalid_argument("La vitesse de décrochage doit être positive.");
    }
    m_vitesseDecrochage = vitesse_kmh / 3.6f;
}

void Avion::setDistanceRestante(float distance) {
    if (distance < 0.f) {
        throw invalid_argument("La distance restante doit être >= 0.");
    }
    m_distanceRestante = distance;
}

void Avion::update(float dt) {
    if (dt <= 0.f) return;

    // Mise à jour de la vitesse : v = v + a*dt
    m_vitesse += m_acceleration * dt;

    // Empêcher la vitesse négative (l'avion ne vole pas en arrière)
    m_vitesse.x = max(0.f, m_vitesse.x);
    m_vitesse.y = max(0.f, m_vitesse.y);

    // Mise à jour de la position : p = p + v*dt
    m_position += m_vitesse * dt;

    float norme =  m_vitesse.norme();
    // Détection simple du décrochage (perte de portance)
    if (norme < m_vitesseDecrochage - EPSILON) {
        cerr << "[AVION] Attention : vitesse en dessous du décrochage (" << norme * 3.6f << " km/h) !\n";
    }

    float deplacementHorizontal = norme * dt;
    m_distanceRestante = max(0.f, m_distanceRestante - deplacementHorizontal);
}