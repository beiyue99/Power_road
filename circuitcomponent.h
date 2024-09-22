#ifndef CIRCUITCOMPONENT_H
#define CIRCUITCOMPONENT_H
#include <QGraphicsItemGroup>
#include <QPen>
#include <qmath.h>

QGraphicsItem* createLamp(char label);
QGraphicsItem* createSwitch(int number, bool isVertical);
QGraphicsItem* createPower(int number);
QGraphicsItem* createLampWithoutLabel();
QGraphicsItem* createSwitchWithoutLabel();
QGraphicsItem* createPowerWithoutLabel();


//Component意为组件。继承自QGraphicsItemGroup
//每个电路组件都是一个ItemGroup
class CircuitComponent : public QGraphicsItemGroup {
public:
    CircuitComponent(QGraphicsItem* parent = nullptr) : QGraphicsItemGroup(parent) {
        // 设置组件为可移动的
        setFlag(QGraphicsItem::ItemIsMovable);
        setFlag(QGraphicsItem::ItemIsSelectable);
    }
};


#endif // CIRCUITCOMPONENT_H
