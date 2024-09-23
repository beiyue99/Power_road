#ifndef CIRCUITCOMPONENT_H
#define CIRCUITCOMPONENT_H

#include <qmath.h>
#include <QPainter>
#include <QGraphicsScene> // 添加这一行
#include <QGraphicsItemGroup>
#include <QPen>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>

// 基础电路组件类的基类，是一个组
class CircuitComponent : public QGraphicsItemGroup {
public:
    CircuitComponent(const QString& name, const QString& type, QGraphicsItem* parent = nullptr);

    QString getName() const { return m_name; }
    QString getType() const { return m_type; }


protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) ;
private:
    QString m_name;
    QString m_type;
};


CircuitComponent* createLamp(char label);
CircuitComponent* createSwitch(int number);
CircuitComponent* createPower(int number);
CircuitComponent* createLampWithoutLabel();
CircuitComponent* createSwitchWithoutLabel();
CircuitComponent* createPowerWithoutLabel();



#endif // CIRCUITCOMPONENT_H
