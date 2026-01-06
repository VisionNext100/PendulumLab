#include "pendulumwidget.h"
#include <QPainter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

PendulumWidget::PendulumWidget(QWidget *parent)
    : QWidget(parent), isPaused(true), initialEnergy(0), airResistanceCoefficient(0.01)
{

    // 创建主布局（垂直布局）
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(5, 5, 5, 5);
    mainLayout->setSpacing(10);

    // 创建控制面板（水平布局）
    QHBoxLayout *controlLayout = new QHBoxLayout();
    controlLayout->setSpacing(15);

    // 添加控制组件
    controlLayout->addWidget(new QLabel("小球数量:"));
    nodeCountSpinBox = new QSpinBox(this);
    nodeCountSpinBox->setRange(1, 10);
    nodeCountSpinBox->setValue(1);
    controlLayout->addWidget(nodeCountSpinBox);

    controlLayout->addWidget(new QLabel("质量(kg):"));
    massSpinBox = new QDoubleSpinBox(this);
    massSpinBox->setRange(1.0, 10.0);
    massSpinBox->setValue(1.0);
    controlLayout->addWidget(massSpinBox);

    controlLayout->addWidget(new QLabel("摆长(m):"));
    lengthSpinBox = new QDoubleSpinBox(this);
    lengthSpinBox->setRange(1.0, 5.0);
    lengthSpinBox->setValue(1.0);
    controlLayout->addWidget(lengthSpinBox);

    controlLayout->addWidget(new QLabel("角度(°):"));
    angleSpinBox = new QDoubleSpinBox(this);
    angleSpinBox->setRange(1.0, 60.0);
    angleSpinBox->setValue(15.0);
    controlLayout->addWidget(angleSpinBox);

    controlLayout->addWidget(new QLabel("阻力系数:"));
    resistanceSpinBox = new QDoubleSpinBox(this);
    resistanceSpinBox->setRange(0.01, 0.05);
    resistanceSpinBox->setValue(0.01);
    resistanceSpinBox->setSingleStep(0.01);
    controlLayout->addWidget(resistanceSpinBox);

    startButton = new QPushButton("开始模拟", this);
    pauseButton = new QPushButton("暂停", this);
    controlLayout->addWidget(startButton);
    controlLayout->addWidget(pauseButton);

    // 将控制面板添加到主布局底部
    mainLayout->addStretch(1);  // 添加弹性空间将控制面板推到底部
    mainLayout->addLayout(controlLayout);

    // 初始化其他组件
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PendulumWidget::updatePhysics);
    connect(startButton, &QPushButton::clicked, this, &PendulumWidget::onStartClicked);
    connect(pauseButton, &QPushButton::clicked, this, &PendulumWidget::onPauseClicked);

    initializePendulums();
}

PendulumWidget::~PendulumWidget() {
    if(timer->isActive()) timer->stop();
}

void PendulumWidget::initializePendulums() {
    nodes.clear();
    const int count = nodeCountSpinBox->value();
    const qreal totalLength = lengthSpinBox->value() * PIXELS_PER_METER;
    segmentLength = totalLength / count;
    const qreal angle = angleSpinBox->value() * M_PI / 180.0;

    airResistanceCoefficient = resistanceSpinBox->value();

    pivotPoint = QPointF(width()/2, 50);

    for (int i = 0; i < count; ++i) {
        PendulumNode node;
        node.mass = massSpinBox->value();

        // 修正后的位置计算
        qreal segmentRatio = static_cast<qreal>(i+1)/count;
        qreal x = pivotPoint.x() + totalLength * sin(angle) * segmentRatio;
        qreal y = pivotPoint.y() + totalLength * cos(angle) * segmentRatio;  // 使用cos而不是1-cos

        node.position = node.previousPosition = QPointF(x, y);
        nodes.push_back(node);
    }

    // 计算初始能量
    qreal kinetic, potential;
    calculateEnergies(kinetic, potential);
    initialEnergy = kinetic + potential;
}

void PendulumWidget::updatePhysics() {
    const qreal dt = 0.016;
    const qreal g = GRAVITY * PIXELS_PER_METER;

    // 获取当前空气阻力系数
    airResistanceCoefficient = resistanceSpinBox->value();

    for (auto& node : nodes) {
        QPointF velocity = (node.position - node.previousPosition) / dt;

        // 计算空气阻力（与速度平方成正比）
        qreal speed = sqrt(velocity.x() * velocity.x() + velocity.y() * velocity.y());
        QPointF airResistance = -airResistanceCoefficient * speed * velocity;

        // 更新加速度（重力 + 空气阻力）
        QPointF acceleration(0, g);
        acceleration += airResistance / node.mass;

        // 更新位置
        node.previousPosition = node.position;
        node.position += velocity * dt + acceleration * dt * dt;
    }

    applyConstraints();
    update();
}

void PendulumWidget::applyConstraints() {
    // 约束到固定点
    if (!nodes.empty()) {
        QPointF delta = nodes[0].position - pivotPoint;
        qreal dist = sqrt(delta.x()*delta.x() + delta.y()*delta.y());
        if (dist > segmentLength) {
            nodes[0].position = pivotPoint + delta * (segmentLength/dist);
        }
    }

    // 节点间约束
    for (size_t i = 0; i < nodes.size()-1; ++i) {
        QPointF delta = nodes[i+1].position - nodes[i].position;
        qreal dist = sqrt(delta.x()*delta.x() + delta.y()*delta.y());
        if (dist > segmentLength) {
            QPointF correction = delta * (1 - segmentLength/dist);
            nodes[i+1].position -= correction * 0.5;
            nodes[i].position += correction * 0.5;
        }
    }
}

void PendulumWidget::calculateEnergies(qreal &kinetic, qreal &potential) const {
    kinetic = potential = 0;
    const qreal dt = 0.016;
    const qreal pixelToMeter = 1.0 / PIXELS_PER_METER;

    // 计算平衡位置（所有节点能达到的最大y值）
    qreal equilibriumY = pivotPoint.y() + lengthSpinBox->value() * PIXELS_PER_METER;

    // 计算动能和势能
    for (const auto& node : nodes) {
        // 动能计算（保持不变）
        QPointF velocity = (node.position - node.previousPosition) * pixelToMeter / dt;
        kinetic += 0.5 * node.mass * QPointF::dotProduct(velocity, velocity);

        // 势能计算（相对于平衡位置）
        // 注意：y轴向下为正，所以用 equilibriumY - node.position.y()
        potential += node.mass * GRAVITY *
                     (equilibriumY - node.position.y()) * pixelToMeter;
    }
}

void PendulumWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::white);

    // 绘制固定点
    painter.setPen(QPen(Qt::black, 3));
    painter.drawPoint(pivotPoint);

    // 绘制摆线
    painter.setPen(QPen(Qt::black, 2));
    if (!nodes.empty()) {
        painter.drawLine(pivotPoint, nodes[0].position);
        for (size_t i = 0; i < nodes.size()-1; ++i) {
            painter.drawLine(nodes[i].position, nodes[i+1].position);
        }
    }

    // 绘制小球
    painter.setBrush(Qt::red);
    for (const auto& node : nodes) {
        painter.drawEllipse(node.position, 10, 10);
    }

    // 显示参数
    qreal kinetic, potential;
    calculateEnergies(kinetic, potential);

    painter.setPen(Qt::black);
    int yPos = 30;
    painter.drawText(10, yPos, QString("小球数量: %1").arg(nodes.size())); yPos += 20;
    painter.drawText(10, yPos, QString("系统动能: %1 J").arg(kinetic, 0, 'f', 3)); yPos += 20;
    painter.drawText(10, yPos, QString("系统势能: %1 J").arg(potential, 0, 'f', 3)); yPos += 20;
    painter.drawText(10, yPos, QString("机械能: %1 J").arg(kinetic + potential, 0, 'f', 3)); yPos += 20;
    //painter.drawText(10, yPos, QString("空气阻力k: %1").arg(airResistanceCoefficient, 0, 'f', 3)); yPos += 20;

    //if (!nodes.empty()) {
    //qreal period = 2 * M_PI * sqrt(lengthSpinBox->value() / GRAVITY);
    //painter.drawText(10, yPos, QString("理论周期: %1 s").arg(period, 0, 'f', 3));
    //}
}

void PendulumWidget::startSimulation() {
    initializePendulums();
    if (!timer->isActive()) {
        timer->start(16);
    }
    isPaused = false;
    pauseButton->setText("暂停");
}

void PendulumWidget::pauseSimulation() {
    isPaused = !isPaused;
    if (isPaused) {
        timer->stop();
        pauseButton->setText("继续");
    } else {
        timer->start(16);
        pauseButton->setText("暂停");
    }
}

void PendulumWidget::onStartClicked() {
    startSimulation();
}

void PendulumWidget::onPauseClicked() {
    pauseSimulation();
}

void PendulumWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    if (!nodes.empty()) {
        initializePendulums();
    }
}

void PendulumWidget::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    if (nodes.empty()) {
        initializePendulums();
    }
}
