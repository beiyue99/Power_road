#include "circuitcomponent.h"



// 自定义灯泡图形，添加编号
QGraphicsItem* createLamp(char label) {
    CircuitComponent* lamp = new CircuitComponent;

    // 创建外部空心圆圈
    QGraphicsEllipseItem* bulbOuterCircle = new QGraphicsEllipseItem(0, 0, 40, 40);
    bulbOuterCircle->setPen(QPen(Qt::black, 2));    // 设置边框
    bulbOuterCircle->setBrush(Qt::NoBrush);         // 确保没有填充
    lamp->addToGroup(bulbOuterCircle);

    // 添加六条实线，从圆周向外延伸
    for (int i = 0; i < 6; ++i) {
        // 计算每条线的起点（在圆周上）和终点（在圆圈外）
        qreal angle = i * M_PI / 3;  // 每隔60度一条线
        qreal startX = 20 + 20 * qCos(angle);
        qreal startY = 20 + 20 * qSin(angle);
        qreal endX = 20 + 30 * qCos(angle);
        qreal endY = 20 + 30 * qSin(angle);

        QGraphicsLineItem* line = new QGraphicsLineItem(QLineF(startX, startY, endX, endY));
        line->setPen(QPen(Qt::black, 2));
        lamp->addToGroup(line);
    }

    // 在灯泡下方添加字母编号
    QGraphicsTextItem* labelItem = new QGraphicsTextItem(QString(label));
    labelItem->setPos(15, 45);  // 设置字母的位置
    lamp->addToGroup(labelItem);

    lamp->setFlag(QGraphicsItem::ItemIsSelectable, false);
    return lamp;
}






QGraphicsItem* createSwitch(int number, bool isVertical) {
    CircuitComponent* group = new CircuitComponent;

    // 两个小圆圈
    QGraphicsEllipseItem* circle1 = new QGraphicsEllipseItem(0, 0, 10, 10);
    circle1->setPen(QPen(Qt::black, 2));
    QGraphicsEllipseItem* circle2 = new QGraphicsEllipseItem(50, 0, 10, 10);
    circle2->setPen(QPen(Qt::black, 2));

    group->addToGroup(circle1);
    group->addToGroup(circle2);

    // 圆圈外两端的实线
    QGraphicsLineItem* line1 = new QGraphicsLineItem(QLineF(-10, 5, 0, 5));  // 左端实线
    line1->setPen(QPen(Qt::black, 2));
    group->addToGroup(line1);

    QGraphicsLineItem* line2 = new QGraphicsLineItem(QLineF(60, 5, 70, 5));  // 右端实线
    line2->setPen(QPen(Qt::black, 2));
    group->addToGroup(line2);

    // 中间的翘起实线，表示未闭合，翘起角度为30度
    QGraphicsLineItem* line = new QGraphicsLineItem(QLineF(5, 5, 45, -10));
    line->setPen(QPen(Qt::black, 2));
    group->addToGroup(line);

    // 添加数字编号，根据开关的方向确定位置
    QGraphicsTextItem* numberItem = new QGraphicsTextItem(QString::number(number));
    if (isVertical) {
        numberItem->setPos(55, 0);  // 竖直时，数字在右侧
    } else {
        numberItem->setPos(20, 15);  // 水平时，数字在下方
    }
    group->addToGroup(numberItem);

    group->setFlag(QGraphicsItem::ItemIsSelectable, false);
    return group;
}



// 自定义电源图形，添加编号
QGraphicsItem* createPower(int number) {
    CircuitComponent* group = new CircuitComponent;

    // 外部圆圈
    QGraphicsEllipseItem* powerCircle = new QGraphicsEllipseItem(0, 0, 40, 40);
    powerCircle->setPen(QPen(Qt::black, 2));
    group->addToGroup(powerCircle);

    // 中间的“S”形实线
    QPainterPath sShape;
    sShape.moveTo(10, 30);
    sShape.cubicTo(10, 0, 30, 40, 30, 10);
    QGraphicsPathItem* sItem = new QGraphicsPathItem(sShape);
    sItem->setPen(QPen(Qt::black, 2));
    group->addToGroup(sItem);

    // 在电源图形左侧添加矩形框和编号
    QGraphicsRectItem* rect = new QGraphicsRectItem(-50, 10, 50, 25);  // 矩形框的位置调整到左侧
    rect->setPen(QPen(Qt::black, 1));
    group->addToGroup(rect);

    QGraphicsTextItem* numberItem = new QGraphicsTextItem(QString("电源") + QString::number(number));
    numberItem->setPos(-45, 10);  // 调整编号的位置到左侧
    group->addToGroup(numberItem);

    group->setFlag(QGraphicsItem::ItemIsSelectable, false);
    return group;
}


// 创建不带编号的灯泡图案
QGraphicsItem* createLampWithoutLabel() {
    CircuitComponent* lamp = new CircuitComponent;

    // 外部空心圆圈
    QGraphicsEllipseItem* bulbOuterCircle = new QGraphicsEllipseItem(0, 0, 40, 40);
    bulbOuterCircle->setPen(QPen(Qt::black, 2));    // 设置边框
    bulbOuterCircle->setBrush(Qt::NoBrush);         // 确保没有填充
    lamp->addToGroup(bulbOuterCircle);

    // 绘制从圆周向外延伸的六条实线
    for (int i = 0; i < 6; ++i) {
        qreal angle = i * 60;  // 每条线相隔60度
        qreal x1 = 20 + 20 * qCos(qDegreesToRadians(angle));  // 起点在圆周上
        qreal y1 = 20 + 20 * qSin(qDegreesToRadians(angle));
        qreal x2 = 20 + 30 * qCos(qDegreesToRadians(angle));  // 终点在圆外
        qreal y2 = 20 + 30 * qSin(qDegreesToRadians(angle));
        lamp->addToGroup(new QGraphicsLineItem(x1, y1, x2, y2, bulbOuterCircle));
    }
    lamp->setFlag(QGraphicsItem::ItemIsSelectable, false); // 禁止选中
    return lamp;
}


QGraphicsItem* createSwitchWithoutLabel() {
    CircuitComponent* group = new CircuitComponent;

    // 两个小圆圈
    QGraphicsEllipseItem* circle1 = new QGraphicsEllipseItem(0, 0, 10, 10);
    circle1->setPen(QPen(Qt::black, 2));
    QGraphicsEllipseItem* circle2 = new QGraphicsEllipseItem(50, 0, 10, 10);
    circle2->setPen(QPen(Qt::black, 2));

    group->addToGroup(circle1);
    group->addToGroup(circle2);

    // 圆圈外两端的实线
    QGraphicsLineItem* line1 = new QGraphicsLineItem(QLineF(-10, 5, 0, 5));  // 左端实线
    line1->setPen(QPen(Qt::black, 2));
    group->addToGroup(line1);

    QGraphicsLineItem* line2 = new QGraphicsLineItem(QLineF(60, 5, 70, 5));  // 右端实线
    line2->setPen(QPen(Qt::black, 2));
    group->addToGroup(line2);

    // 中间的翘起实线，表示未闭合，翘起角度为30度
    QGraphicsLineItem* line = new QGraphicsLineItem(QLineF(5, 5, 45, -10));
    line->setPen(QPen(Qt::black, 2));
    group->addToGroup(line);

    group->setFlag(QGraphicsItem::ItemIsSelectable, false);
    return group;
}

// 不带标识的电源图形
QGraphicsItem* createPowerWithoutLabel() {
    CircuitComponent* group = new CircuitComponent;

    // 外部圆圈
    QGraphicsEllipseItem* powerCircle = new QGraphicsEllipseItem(0, 0, 40, 40);
    powerCircle->setPen(QPen(Qt::black, 2));

    // 中间的“S”形实线
    QPainterPath sShape;
    sShape.moveTo(10, 30);
    sShape.cubicTo(10, 0, 30, 40, 30, 10);
    QGraphicsPathItem* sItem = new QGraphicsPathItem(sShape);
    sItem->setPen(QPen(Qt::black, 2));

    // 添加到组中
    group->addToGroup(powerCircle);
    group->addToGroup(sItem);

    group->setFlag(QGraphicsItem::ItemIsSelectable, false); // 禁止选中
    // 返回没有标识符的电源图形
    return group;
}
