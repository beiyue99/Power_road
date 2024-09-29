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
#include <QDebug>
// 基础电路组件类的基类，是一个组
class CircuitComponent : public QGraphicsItemGroup {
public:
    CircuitComponent(const QString& name, const QString& type, QGraphicsItem* parent = nullptr);

    QString getName() const { return m_name; }
    QString getType() const { return m_type; }
    void setRotation(double angle) ;
    // 使用 父类QGraphicsItemGroup 的 setRotation 方法，实现整个组的旋转

    // 新增开关状态
    bool isClosed() const { return m_isClosed; }
    void setClosed(bool closed) ;
    // 判断当前状态是水平还是垂直
//    bool isVertical() const { return rotation() == 270 || rotation() == -90; }
    // 更新开关的显示
    void updateSwitchAppearance();
protected:
    //重写绘制函数，当绘制不同形状，使用不同的画笔
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) ;
private:
    QString m_name;
    QString m_type;
    bool m_isClosed = false;


};




CircuitComponent* createLamp(char label);
CircuitComponent* createSwitch(int number);
CircuitComponent* createPower(int number);
CircuitComponent* createLampWithoutLabel();
CircuitComponent* createSwitchWithoutLabel();
CircuitComponent* createPowerWithoutLabel();



#endif // CIRCUITCOMPONENT_H
