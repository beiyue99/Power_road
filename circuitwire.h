#ifndef CIRCUITWIRE_H
#define CIRCUITWIRE_H

#include <QGraphicsLineItem>
#include <QString>
#include "circuitcomponent.h"


class CircuitComponent;
class CircuitWire : public QGraphicsLineItem {
public:
    CircuitWire(QGraphicsItem* startItem, QGraphicsItem* endItem,
               const QString& startEndType, const QString& endEndType,
               QGraphicsItem* parent = nullptr);

    void updatePosition();
    QString getStartEndType() const { return m_startEndType; }
    QString getEndEndType() const { return m_endEndType; }

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
    QPointF calculateCircleIntersection(CircuitComponent* lampComponent, const QPointF& otherPos) const;

    QGraphicsItem* m_startItem;
    QGraphicsItem* m_endItem;
    QString m_startEndType;
    QString m_endEndType;
};

#endif // CIRCUITWIRE_H
