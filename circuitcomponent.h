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
#include "circuitwire.h"
#include <QObject>
#include <QComboBox>

class CircuitWire;
// 基础电路组件类的基类，是一个组
class CircuitComponent : public QObject, public QGraphicsItemGroup {
    Q_OBJECT
public:
    CircuitComponent(const QString& name, const QString& type, QGraphicsItem* parent = nullptr);

    QString getName() const { return m_name; }
    QString getType() const { return m_type; }
    void setRotation(double angle); // 使用 父类QGraphicsItemGroup 的 setRotation 方法，实现整个组的旋转
    // 如果 getWires() 函数声明为 const 版本，改为非常量版本
    QMap<QString, QList<CircuitWire*>>& getWires() {
        return m_wires;
    }

    // 新增开关状态
    bool isClosed() const { return m_isClosed; }
    void setClosed(bool closed);

    // 更新开关的显示
    void updateSwitchAppearance();

    void addWire(const QString& end, CircuitWire* wire);
    void removeWire(const QString& end);
    void updateWires();
    QPointF getWireEndPosition(const QString& end) const; // 新增方法

    void addToGroup(QGraphicsItem* item);
    void removeAllWires();
    void removeAllWires(const QString& end);

protected:
    // 重写绘制函数，当绘制不同形状，使用不同的画笔
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

public:
    QString m_name;
    QString m_type;
    bool m_isClosed = false;
    QMap<QString, QList<CircuitWire*>> m_wires; // 修改为每个端点对应一个连线列表

    // 添加组合框和连接状态
    QStringList comboBoxValues;  // 存储组合框的值，6个元素
    QComboBox* comboBoxes[6];    // 指向对应的组合框
public:
    const QMap<QString, QList<CircuitWire*>>& getWires() const { return m_wires; }

    void setComboBoxes(QComboBox* boxes[6]) {
          for (int i = 0; i < 6; ++i) {
              comboBoxes[i] = boxes[i];
          }
      }
public:
    qreal getRadius() const; // 新增方法，获取组件的半径

signals:
    void positionChanged();  // 添加位置变化的信号
    void switchStateChanged(); // 添加开关状态变化的信号
};

// 声明创建组件的函数
CircuitComponent* createLamp(const QString& label);
CircuitComponent* createSwitch(int number);
CircuitComponent* createPower(int number);
CircuitComponent* createLampWithoutLabel();
CircuitComponent* createSwitchWithoutLabel();
CircuitComponent* createPowerWithoutLabel();

#endif // CIRCUITCOMPONENT_H
