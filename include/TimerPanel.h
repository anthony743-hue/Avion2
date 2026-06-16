#pragma once
#include <QWidget>

// Panel du timer : affiche le temps écoulé de la simulation.
// Le dessin est réalisé dans DrawTimer (paintEvent).
class TimerPanel : public QWidget {
    Q_OBJECT
public:
    explicit TimerPanel(QWidget *parent = nullptr);

    void setElapsed(double secondes);   // met à jour la valeur affichée
    double elapsed() const { return m_elapsed; }
    void reset();

    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;  // DrawTimer

private:
    double m_elapsed = 0.0;  // secondes
    QString formatTemps() const;
};
