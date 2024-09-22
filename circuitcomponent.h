#ifndef CIRCUITCOMPONENT_H
#define CIRCUITCOMPONENT_H
#include <QGraphicsItemGroup>
#include <QPen>
#include <qmath.h>
#include <QPainter>
#include <QGraphicsScene> // 添加这一行

//Component意为组件。继承自QGraphicsItemGroup
//每个电路组件都是一个ItemGroup

#include <QGraphicsItemGroup>
#include <QPen>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>

// 基础电路组件类
class CircuitComponent : public QGraphicsItemGroup {
public:
    CircuitComponent(const QString& name, const QString& type, QGraphicsItem* parent = nullptr)
        : QGraphicsItemGroup(parent), m_name(name), m_type(type) {
        setFlag(QGraphicsItem::ItemIsMovable);
        setFlag(QGraphicsItem::ItemIsSelectable);
        setHandlesChildEvents(false);
    }

    QString getName() const { return m_name; }
    QString getType() const { return m_type; }

    void mousePressEvent(QGraphicsSceneMouseEvent* event);

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override {
        if (isSelected()) {
            painter->setPen(QPen(Qt::red, 3));
        } else {
            painter->setPen(QPen(Qt::black, 2));
        }
        // 绘制子项
        for (QGraphicsItem* item : childItems()) {
            if (QGraphicsEllipseItem* ellipse = dynamic_cast<QGraphicsEllipseItem*>(item)) {
                painter->drawEllipse(ellipse->rect());
            } else if (QGraphicsLineItem* line = dynamic_cast<QGraphicsLineItem*>(item)) {
                painter->drawLine(line->line());
            }
        }
        QGraphicsItemGroup::paint(painter, option, widget);
    }

private:
    QString m_name;
    QString m_type;
};
// 开关组件类
class SwitchComponent : public CircuitComponent {
public:
    SwitchComponent(const QString& name, bool isVertical, QGraphicsItem* parent = nullptr)
        : CircuitComponent(name, "Switch", parent) {
        createSwitch(isVertical);
    }

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override {
        CircuitComponent::paint(painter, option, widget); // 确保先调用基类的绘制方法

        // 设置边框颜色
        if (isSelected()) {
            painter->setPen(QPen(Qt::red, 3)); // 选中时红色边框
        } else {
            painter->setPen(QPen(Qt::black, 2)); // 默认黑色边框
        }

        // 绘制开关线条
        for (QGraphicsItem* item : childItems()) {
            if (QGraphicsLineItem* line = dynamic_cast<QGraphicsLineItem*>(item)) {
                painter->drawLine(line->line());
            }
        }
    }

private:
    void createSwitch(bool isVertical) {
        QGraphicsLineItem* line = new QGraphicsLineItem(0, 0, isVertical ? 0 : 50, isVertical ? 50 : 0);
        line->setPen(QPen(Qt::black, 2));
        addToGroup(line);
    }
};

// 电源组件类
class PowerComponent : public CircuitComponent {
public:
    PowerComponent(const QString& name, QGraphicsItem* parent = nullptr)
        : CircuitComponent(name, "Power", parent) {
        createPower();
    }

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override {
        CircuitComponent::paint(painter, option, widget); // 确保先调用基类的绘制方法

        // 设置边框颜色
        if (isSelected()) {
            painter->setPen(QPen(Qt::red, 3)); // 选中时红色边框
        } else {
            painter->setPen(QPen(Qt::black, 2)); // 默认黑色边框
        }

        // 绘制电源矩形
        for (QGraphicsItem* item : childItems()) {
            if (QGraphicsRectItem* rect = dynamic_cast<QGraphicsRectItem*>(item)) {
                painter->drawRect(rect->rect());
            }
        }
    }

private:
    void createPower() {
        QGraphicsRectItem* rect = new QGraphicsRectItem(0, 0, 30, 50);
        rect->setPen(QPen(Qt::black, 2));
        rect->setBrush(Qt::yellow);
        addToGroup(rect);
    }
};









CircuitComponent* createLamp(char label);
CircuitComponent* createSwitch(int number, bool isVertical);
CircuitComponent* createPower(int number);
CircuitComponent* createLampWithoutLabel();
CircuitComponent* createSwitchWithoutLabel();
CircuitComponent* createPowerWithoutLabel();



#endif // CIRCUITCOMPONENT_H
