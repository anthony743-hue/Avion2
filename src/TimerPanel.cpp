#include "TimerPanel.h"
#include <QPainter>

TimerPanel::TimerPanel(QWidget *parent) : QWidget(parent) {
    setMinimumSize(180, 70);
}

QSize TimerPanel::sizeHint() const {
    return QSize(220, 80);
}

void TimerPanel::setElapsed(double secondes) {
    m_elapsed = secondes < 0.0 ? 0.0 : secondes;
    update();  // déclenche paintEvent
}

void TimerPanel::reset() {
    setElapsed(0.0);
}

QString TimerPanel::formatTemps() const {
    const int totalCentiemes = static_cast<int>(m_elapsed * 100.0 + 0.5);
    const int minutes = totalCentiemes / 6000;
    const int secondes = (totalCentiemes / 100) % 60;
    const int centiemes = totalCentiemes % 100;
    return QString("%1:%2.%3")
        .arg(minutes, 2, 10, QChar('0'))
        .arg(secondes, 2, 10, QChar('0'))
        .arg(centiemes, 2, 10, QChar('0'));
}

void TimerPanel::paintEvent(QPaintEvent * /*event*/) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    const QRectF zone = rect().adjusted(2, 2, -2, -2);
    painter.setBrush(QColor(20, 24, 32));
    painter.setPen(QPen(QColor(60, 70, 90), 2));
    painter.drawRoundedRect(zone, 8, 8);

    painter.setPen(QColor(150, 160, 180));
    QFont labelFont = painter.font();
    labelFont.setPointSize(9);
    painter.setFont(labelFont);
    painter.drawText(zone.adjusted(10, 6, -10, 0), Qt::AlignLeft | Qt::AlignTop, "TEMPS DE VOL");

    QFont chrono = painter.font();
    chrono.setPointSize(24);
    chrono.setBold(true);
    chrono.setFamily("Monospace");
    painter.setFont(chrono);
    painter.setPen(QColor(120, 230, 160));
    painter.drawText(zone.adjusted(10, 0, -10, -6), Qt::AlignCenter, formatTemps());
}
