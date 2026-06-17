#include "SimulationView.h"
#include <QPainter>
#include <QPainterPath>
#include <algorithm>

SimulationView::SimulationView(QWidget *parent) : QWidget(parent) {
    setMinimumSize(480, 320);
    setAutoFillBackground(true);
}

QSize SimulationView::sizeHint() const {
    return QSize(640, 380);
}

void SimulationView::paintEvent(QPaintEvent * /*event*/) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    const int W = width();
    const int H = height();
    const int margin = 30;
    const int solY = H - margin;  // ligne de sol à l'écran

    // Ciel
    QLinearGradient ciel(0, 0, 0, H);
    ciel.setColorAt(0.0, QColor(135, 196, 235));
    ciel.setColorAt(1.0, QColor(205, 230, 245));
    painter.fillRect(rect(), ciel);

    // Sol
    painter.fillRect(QRect(0, solY, W, H - solY), QColor(110, 160, 95));

    if (!m_avion) {
        painter.setPen(QColor(40, 60, 80));
        QFont f = painter.font();
        f.setPointSize(12);
        painter.setFont(f);
        painter.drawText(rect(), Qt::AlignCenter,
                         "Entrez les données puis cliquez sur « Lancer ».");
        return;
    }

    const Vecteur pos = m_avion->getPosition();

    // Étendue du monde (mètres).
    float worldW = std::max({m_distanceInitiale, pos.x, 1.f}) * 1.08f;
    float maxAlt = std::max(pos.y, 1.f);
    if (m_trajectoire) {
        for (const Vecteur &p : *m_trajectoire)
            maxAlt = std::max(maxAlt, p.y);
    }
    const float worldH = maxAlt * 1.30f;

    auto toScreen = [&](const Vecteur &p) -> QPointF {
        const float sx = margin + (p.x / worldW) * (W - 2 * margin);
        const float sy = solY - (p.y / worldH) * (solY - margin);
        return QPointF(sx, sy);
    };

    // Piste d'atterrissage (au sol, à la distance initiale).
    const float pisteX = margin + (m_distanceInitiale / worldW) * (W - 2 * margin);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(60, 60, 65));
    painter.drawRect(QRectF(pisteX - 4, solY - 6, 70, 10));
    painter.setPen(QPen(QColor(245, 245, 245), 2, Qt::DashLine));
    painter.drawLine(QPointF(pisteX, solY - 1), QPointF(pisteX + 62, solY - 1));
    painter.setPen(QColor(40, 40, 45));
    painter.drawText(QPointF(pisteX - 4, solY + 18), "Piste");

    // Trajectoire parcourue.
    if (m_trajectoire && m_trajectoire->size() > 1) {
        QPainterPath chemin(toScreen(m_trajectoire->front()));
        for (size_t i = 1; i < m_trajectoire->size(); ++i)
            chemin.lineTo(toScreen((*m_trajectoire)[i]));
        painter.setPen(QPen(QColor(200, 60, 60), 2, Qt::DotLine));
        painter.setBrush(Qt::NoBrush);
        painter.drawPath(chemin);
    }

    // Avion (petit triangle orienté vers la droite).
    const QPointF p = toScreen(pos);
    const bool alerte = m_avion->estEnDecrochage();
    painter.setPen(Qt::NoPen);
    painter.setBrush(alerte ? QColor(220, 50, 50) : QColor(30, 40, 70));
    QPolygonF avion;
    avion << QPointF(p.x() + 14, p.y())
          << QPointF(p.x() - 10, p.y() - 7)
          << QPointF(p.x() - 4, p.y())
          << QPointF(p.x() - 10, p.y() + 7);
    painter.drawPolygon(avion);

    // Informations.
    painter.setPen(QColor(20, 30, 50));
    QFont info = painter.font();
    info.setPointSize(10);
    painter.setFont(info);
    const QString txt = QString("Position (%1 m, %2 m)   Vitesse %3 km/h   Distance restante %4 m")
                            .arg(pos.x, 0, 'f', 0)
                            .arg(pos.y, 0, 'f', 0)
                            .arg(m_avion->getVitesseKmh(), 0, 'f', 0)
                            .arg(m_avion->getDistanceRestante(), 0, 'f', 0);
    painter.drawText(QPointF(margin, margin - 8), txt);

    if (alerte) {
        painter.setPen(QColor(200, 30, 30));
        QFont b = painter.font();
        b.setBold(true);
        painter.setFont(b);
        painter.drawText(QPointF(margin, margin + 12), "⚠ DÉCROCHAGE !!!!");
    }
}
