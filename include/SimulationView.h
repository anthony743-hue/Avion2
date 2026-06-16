#pragma once
#include "Avion.h"
#include "Vecteur.h"
#include <QWidget>
#include <vector>

// Vue 2D de la simulation : dessine le sol, la piste d'atterrissage,
// la trajectoire et la position courante de l'avion.
class SimulationView : public QWidget {
    Q_OBJECT
public:
    explicit SimulationView(QWidget *parent = nullptr);

    // Les pointeurs sont possédés par le panel global (non copiés ici).
    void setAvion(const Avion *avion) { m_avion = avion; }
    void setTrajectoire(const std::vector<Vecteur> *points) { m_trajectoire = points; }
    void setDistanceInitiale(float distance) { m_distanceInitiale = distance; }

    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    const Avion *m_avion = nullptr;
    const std::vector<Vecteur> *m_trajectoire = nullptr;
    float m_distanceInitiale = 0.f;
};
