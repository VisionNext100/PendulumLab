#ifndef PENDULUMWIDGET_H
#define PENDULUMWIDGET_H

#include <QWidget>
#include <QTimer>
#include <vector>
#include <QPointF>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QResizeEvent>
#include <QShowEvent>

class PendulumWidget : public QWidget {
    Q_OBJECT
public:
    explicit PendulumWidget(QWidget *parent = nullptr);
    virtual ~PendulumWidget();

    void startSimulation();
    void pauseSimulation();

protected:
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;

private slots:
    void onStartClicked();
    void onPauseClicked();


private:
    QDoubleSpinBox *resistanceSpinBox;
    qreal airResistanceCoefficient;

private:
    struct PendulumNode {
        qreal mass;
        QPointF position;
        QPointF previousPosition;
    };

    void initializePendulums();
    void updatePhysics();
    void applyConstraints();
    void calculateEnergies(qreal &kinetic, qreal &potential) const;

    QTimer *timer;
    std::vector<PendulumNode> nodes;
    QPointF pivotPoint;

    QSpinBox *nodeCountSpinBox;
    QDoubleSpinBox *massSpinBox, *lengthSpinBox, *angleSpinBox;
    QPushButton *startButton, *pauseButton;

    qreal segmentLength;
    qreal initialEnergy;
    bool isPaused;
    const qreal GRAVITY = 9.8;
    const qreal PIXELS_PER_METER = 100.0;
};

#endif // PENDULUMWIDGET_H
