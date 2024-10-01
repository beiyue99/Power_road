#include "circuitcomponent.h"



void CircuitComponent::setClosed(bool closed) {
    m_isClosed = closed;  // 更新内部的闭合状态
    updateSwitchAppearance();  // 更新开关外观
}


void CircuitComponent::updateSwitchAppearance() {
    QGraphicsLineItem* middleLine = nullptr;

    // 查找中间的翘起线
    for (QGraphicsItem* item : childItems()) {
        if (QGraphicsLineItem* line = dynamic_cast<QGraphicsLineItem*>(item)) {
            middleLine = line;
        }
    }

    if (middleLine) {

        if (m_isClosed) {
            // 水平状态下的闭合
            middleLine->setLine(QLineF(5, 5, 55, 5));  // 水平方向闭合
        } else {
            // 水平状态下的断开，翘起30度
            middleLine->setLine(QLineF(5, 5, 55, -10));  // 向上翘起30度
        }
    }
    update();  // 更新显示
}






void CircuitComponent::removeAllWires() {
    for (const QString& end : m_wires.keys()) {
        removeAllWires(end);
    }
    m_wires.clear();
}

void CircuitComponent::removeAllWires(const QString& end) {
    if (m_wires.contains(end)) {
        for (CircuitWire* wire : m_wires[end]) {
            if (wire->scene()) {
                wire->scene()->removeItem(wire);
            }
            delete wire;
        }
        m_wires[end].clear();
    }
}

void CircuitComponent::addWire(const QString& end, CircuitWire* wire) {
    m_wires[end].append(wire);
}

void CircuitComponent::updateWires()
{
    for (const QList<CircuitWire*>& wireList : m_wires) {
        for (CircuitWire* wire : wireList) {
            wire->updatePosition();
        }
    }
}



// 重写 itemChange 事件
QVariant CircuitComponent::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionHasChanged) {
        qDebug() << "Item position changed:" << this->getName();
        updateWires();  // 更新连线
        emit positionChanged();  // 发射位置变化的信号
    }
    return QGraphicsItemGroup::itemChange(change, value);
}



void CircuitComponent::addToGroup(QGraphicsItem* item) {
    item->setAcceptedMouseButtons(Qt::NoButton); // 禁用子项的鼠标事件
    QGraphicsItemGroup::addToGroup(item);
}



void CircuitComponent::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    qDebug() << "Component moved:" << getName();
    QGraphicsItemGroup::mouseMoveEvent(event);
    updateWires();  // 更新连线
    emit positionChanged();  // 发射位置变化的信号
}





QPointF CircuitComponent::getWireEndPosition(const QString& end) const
{
    if (m_type == "开关") {
        if (end == "1端") {
            return mapToScene(QPointF(-10, 5)); // 左端
        } else if (end == "2端") {
            return mapToScene(QPointF(70, 5)); // 右端
        }
    } else {
        // 对于灯泡和电源，返回组件的中心位置
        return mapToScene(boundingRect().center());
    }
}




void CircuitComponent::setRotation(double angle) {

    QGraphicsItemGroup::setRotation(angle);
    // 确保更新视觉效果
    update();
}




CircuitComponent::CircuitComponent(const QString& name, const QString& type, QGraphicsItem* parent)
    : QGraphicsItemGroup(parent), m_name(name), m_type(type) {
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setHandlesChildEvents(false);
}



void CircuitComponent::paint(QPainter* painter,
                             const QStyleOptionGraphicsItem* option, QWidget* widget) {
    // 选中时将元件设置为红色
    QPen pen;
    if (isSelected()) {
        pen = QPen(Qt::red, 2);              // 设置线条颜色为红色
    } else {
        pen = QPen(Qt::black, 2);                    // 默认线条颜色为黑色
    }

    // 绘制子项
    for (QGraphicsItem* item : childItems()) {
          painter->setPen(pen);
          if (QGraphicsEllipseItem* ellipse = dynamic_cast<QGraphicsEllipseItem*>(item)) {
              // 绘制椭圆
              painter->drawEllipse(ellipse->rect());
          } else if (QGraphicsLineItem* line = dynamic_cast<QGraphicsLineItem*>(item)) {
              // 绘制线条
              painter->drawLine(line->line());
          } else if (QGraphicsPathItem* path = dynamic_cast<QGraphicsPathItem*>(item)) {
              // 绘制“S”形路径
              painter->drawPath(path->path());
          }
      }
}





CircuitComponent* createSwitch(int number) {
    CircuitComponent* group = new CircuitComponent(QString("开关%1").arg(number), "开关");

    // 两个小圆圈
    QGraphicsEllipseItem* circle1 = new QGraphicsEllipseItem(0, 0, 10, 10);
    QGraphicsEllipseItem* circle2 = new QGraphicsEllipseItem(50, 0, 10, 10);
    group->addToGroup(circle1);
    group->addToGroup(circle2);

    // 圆圈外两端的实线
    QGraphicsLineItem* line1 = new QGraphicsLineItem(QLineF(-10, 5, 0, 5));  // 左端实线
    group->addToGroup(line1);
    QGraphicsLineItem* line2 = new QGraphicsLineItem(QLineF(60, 5, 70, 5));  // 右端实线
    group->addToGroup(line2);

    // 中间的翘起实线，表示未闭合，翘起角度为30度
    QGraphicsLineItem* middleLine = new QGraphicsLineItem(QLineF(5, 5, 55, -10));
    group->addToGroup(middleLine);

    // 添加数字编号
    QGraphicsTextItem* numberItem = new QGraphicsTextItem(QString::number(number));
    numberItem->setPos(20, 15);  // 数字在下方
    group->addToGroup(numberItem);

    // 初始化为断开状态
    group->setClosed(false);

    return group;
}














CircuitComponent* createLamp(char label) {
    // 创建一个 CircuitComponent，表示灯泡
    CircuitComponent* lampComponent = new CircuitComponent(QString("灯泡%1").arg(label), "灯泡");

    // 创建灯泡的空心部分
    QGraphicsEllipseItem* lampBody = new QGraphicsEllipseItem(0, 0, 50, 50);
    lampBody->setAcceptedMouseButtons(Qt::NoButton); // 禁用鼠标事件
    lampComponent->addToGroup(lampBody);

    // 定义灯泡中心和外圆半径
    const qreal centerX = 25;
    const qreal centerY = 25;
    const qreal radius = 25;
    const qreal lineLength = 20; // 每根线的长度

    // 创建六根线，分别按60度间隔排列
    for (int i = 0; i < 6; ++i) {
        // 计算每根线的角度 (60度间隔)
        qreal angle = qDegreesToRadians(60.0 * i);
        // 计算每根线的起点（灯泡外圆上的点）
        qreal startX = centerX + radius * qCos(angle);
        qreal startY = centerY + radius * qSin(angle);
        // 计算每根线的终点（起点再延伸lineLength）
        qreal endX = startX + lineLength * qCos(angle);
        qreal endY = startY + lineLength * qSin(angle);
        // 创建线条并添加到组件中
        QGraphicsLineItem* line = new QGraphicsLineItem(startX, startY, endX, endY);
        line->setAcceptedMouseButtons(Qt::NoButton); // 禁用鼠标事件
        lampComponent->addToGroup(line);
    }

    // 创建文本标签
    QGraphicsTextItem* lampLabel = new QGraphicsTextItem(QString(label));
    lampLabel->setPos(20, 20); // 设置标签位置
    lampLabel->setAcceptedMouseButtons(Qt::NoButton); // 禁用鼠标事件
    lampComponent->addToGroup(lampLabel);

    return lampComponent;
}








// 自定义电源图形，添加编号
CircuitComponent* createPower(int number) {
    CircuitComponent* group = new CircuitComponent(QString("电源%1").arg(number), "电源");

    // 外部圆圈
    QGraphicsEllipseItem* powerCircle = new QGraphicsEllipseItem(0, 0, 40, 40);
    powerCircle->setAcceptedMouseButtons(Qt::NoButton); // 禁用鼠标事件
    group->addToGroup(powerCircle);

    // 中间的“S”形实线
    QPainterPath sShape;
    sShape.moveTo(10, 30);
    sShape.cubicTo(10, 0, 30, 40, 30, 10);
    QGraphicsPathItem* sItem = new QGraphicsPathItem(sShape);
    sItem->setAcceptedMouseButtons(Qt::NoButton); // 禁用鼠标事件
    group->addToGroup(sItem);

    // 在电源图形左侧添加矩形框和编号
    QGraphicsRectItem* rect = new QGraphicsRectItem(-50, 10, 50, 25);
    rect->setAcceptedMouseButtons(Qt::NoButton); // 禁用鼠标事件
    group->addToGroup(rect);

    QGraphicsTextItem* numberItem = new QGraphicsTextItem(QString("电源%1").arg(number));
    numberItem->setPos(-45, 10);
    numberItem->setAcceptedMouseButtons(Qt::NoButton); // 禁用鼠标事件
    group->addToGroup(numberItem);

    return group;
}


CircuitComponent* createLampWithoutLabel() {
    CircuitComponent* lamp = new CircuitComponent("无标识灯泡", "灯泡");

    // 外部空心圆圈
    QGraphicsEllipseItem* bulbOuterCircle = new QGraphicsEllipseItem(0, 0, 40, 40);
    bulbOuterCircle->setPen(QPen(Qt::black, 2));
    bulbOuterCircle->setBrush(Qt::NoBrush);
    bulbOuterCircle->setAcceptedMouseButtons(Qt::NoButton); // 禁用鼠标事件
    lamp->addToGroup(bulbOuterCircle);

    // 绘制从圆周向外延伸的六条实线
    QPen pen(Qt::black, 2);  // 黑色实线，线宽为2
    for (int i = 0; i < 6; ++i) {
        qreal angle = i * 60;  // 每条线相隔60度
        qreal x1 = 20 + 20 * qCos(qDegreesToRadians(angle));  // 起点在圆周上
        qreal y1 = 20 + 20 * qSin(qDegreesToRadians(angle));
        qreal x2 = 20 + 30 * qCos(qDegreesToRadians(angle));  // 终点在圆外
        qreal y2 = 20 + 30 * qSin(qDegreesToRadians(angle));
        QGraphicsLineItem* lineItem = new QGraphicsLineItem(x1, y1, x2, y2);
        lineItem->setPen(pen);  // 设置线的颜色和宽度
        lineItem->setAcceptedMouseButtons(Qt::NoButton); // 禁用鼠标事件
        lamp->addToGroup(lineItem);
    }

    return lamp;
}




CircuitComponent* createSwitchWithoutLabel() {
    CircuitComponent* group = new CircuitComponent("无标识开关", "开关");
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



CircuitComponent* createPowerWithoutLabel() {
    CircuitComponent* group = new CircuitComponent("无标识电源", "电源");
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
    return group;
}
