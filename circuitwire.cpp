#include "circuitwire.h"
#include "circuitcomponent.h"
#include <QPainter>
#include <QDebug>
#include <QtMath>

CircuitWire::CircuitWire(QGraphicsItem* startItem, QGraphicsItem* endItem,
                         const QString& startEndType, const QString& endEndType,
                         QGraphicsItem* parent)
    : QGraphicsLineItem(parent), m_startItem(startItem),
      m_endItem(endItem), m_startEndType(startEndType),
      m_endEndType(endEndType) {
    setPen(QPen(Qt::black, 2)); // 设置线条颜色和宽度
    updatePosition();
}

void CircuitWire::updatePosition() {
    if (m_startItem && m_endItem) {
        QPointF startPos, endPos;

        // 获取起始组件和终点组件
        CircuitComponent* startComponent = dynamic_cast<CircuitComponent*>(m_startItem);
        CircuitComponent* endComponent = dynamic_cast<CircuitComponent*>(m_endItem);

        // 获取起始位置
        if (startComponent) {
            startPos = startComponent->getWireEndPosition(m_startEndType);
        } else {
            startPos = m_startItem->scenePos();
        }

        // 获取终点位置
        if (endComponent) {
            endPos = endComponent->getWireEndPosition(m_endEndType);
        } else {
            endPos = m_endItem->scenePos();
        }

        // 如果起点是灯泡，调整起点到圆周上
        if (startComponent && startComponent->getType() == "灯泡") {
            startPos = calculateCircleIntersection(startComponent, endPos);
        }

        // 如果终点是灯泡，调整终点到圆周上
        if (endComponent && endComponent->getType() == "灯泡") {
            endPos = calculateCircleIntersection(endComponent, startPos);
        }

        // 设置新的线条
        QLineF newLine(startPos, endPos);
        setLine(newLine);

        // 调试输出
        qDebug() << "CircuitWire::updatePosition()";
        qDebug() << "Start Component:" << (startComponent ? startComponent->getName() : "None")
                 << "Start Pos:" << startPos;
        qDebug() << "End Component:" << (endComponent ? endComponent->getName() : "None")
                 << "End Pos:" << endPos;
    }
}

QPointF CircuitWire::calculateCircleIntersection(CircuitComponent* lampComponent, const QPointF& otherPos) const {
    // 获取灯泡的中心和半径
    QPointF center = lampComponent->mapToScene(QPointF(0, 0));
    qreal radius = lampComponent->getRadius(); // 获取正确的半径

    // 计算从灯泡中心指向另一点的向量
    QPointF direction = otherPos - center;
    qreal length = std::hypot(direction.x(), direction.y());

    if (length == 0) {
        // 避免除以零，返回中心点
        return center;
    }

    // 归一化方向向量
    direction /= length;

    // 计算圆周上的交点
    QPointF intersectionPoint = center + direction * radius;

    // 调试输出
    qDebug() << "calculateCircleIntersection:";
    qDebug() << "Lamp Center:" << center;
    qDebug() << "Other Pos:" << otherPos;
    qDebug() << "Direction:" << direction;
    qDebug() << "Intersection Point:" << intersectionPoint;

    return intersectionPoint;
}

void CircuitWire::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    QPen pen(Qt::black, 2);
    painter->setPen(pen);

    QPointF startPoint = line().p1();
    QPointF endPoint = line().p2();

    // 计算中间点，形成直角折线
    QPointF midPoint1(startPoint.x(), (startPoint.y() + endPoint.y()) / 2);
    QPointF midPoint2(endPoint.x(), (startPoint.y() + endPoint.y()) / 2);

    // 绘制折线
    painter->drawLine(startPoint, midPoint1);
    painter->drawLine(midPoint1, midPoint2);
    painter->drawLine(midPoint2, endPoint);
}
