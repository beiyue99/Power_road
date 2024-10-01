#include "circuitwire.h"
#include "circuitcomponent.h"

CircuitWire::CircuitWire(QGraphicsItem* startItem, QGraphicsItem* endItem, const QString& startEndType, const QString& endEndType, QGraphicsItem* parent)
    : QGraphicsLineItem(parent), m_startItem(startItem), m_endItem(endItem), m_startEndType(startEndType), m_endEndType(endEndType) {
    setPen(QPen(Qt::black, 2)); // 设置线条颜色和宽度
    updatePosition();
}

void CircuitWire::updatePosition() {
    if (m_startItem && m_endItem) {
        QPointF startPos, endPos;

        // 计算起始位置
        CircuitComponent* startComponent = dynamic_cast<CircuitComponent*>(m_startItem);
        if (startComponent) {
            startPos = startComponent->getWireEndPosition(m_startEndType);
        }

        // 计算终点位置
        CircuitComponent* endComponent = dynamic_cast<CircuitComponent*>(m_endItem);
        if (endComponent) {
            endPos = endComponent->getWireEndPosition(m_endEndType);
        }

        QLineF line(startPos, endPos);
        setLine(line);
    }
}




void CircuitWire::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
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
