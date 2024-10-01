#ifndef CIRCUITWIRE_H
#define CIRCUITWIRE_H

#include <QGraphicsLineItem>
#include <QPen>

class CircuitWire : public QGraphicsLineItem {
public:
    CircuitWire(QGraphicsItem* startItem, QGraphicsItem* endItem, const QString& startEndType, const QString& endEndType, QGraphicsItem* parent = nullptr);

    void updatePosition();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
private:
    QGraphicsItem* m_startItem;
    QGraphicsItem* m_endItem;
    QString m_startEndType;
    QString m_endEndType;
};

#endif // CIRCUITWIRE_H
