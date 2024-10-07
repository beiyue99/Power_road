#include "circuitcomponent.h"



// 构造函数
CircuitComponent::CircuitComponent(const QString& name, const QString& type, QGraphicsItem* parent)
    : QObject(), QGraphicsItemGroup(parent), m_name(name), m_type(type) {
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges); // 确保位置变化时发送通知
    setHandlesChildEvents(false);
}



qreal CircuitComponent::getRadius() const {
    if (m_type == "灯泡") {
        return 15.0; // 灯泡半径
    }
    else if (m_type == "电源") {
        return 20.0; // 电源半径（根据实际绘制的圆圈大小调整）
    }
    // 如果有其他圆形组件，可以在此处添加相应的半径
    return 0.0; // 默认非圆形组件
}


void CircuitComponent::setRotation(double angle) {
    QGraphicsItemGroup::setRotation(angle);
    // 确保更新视觉效果
    update();
}

void CircuitComponent::setClosed(bool closed) {
    m_isClosed = closed;  // 更新内部的闭合状态
    updateSwitchAppearance();  // 更新开关外观
    emit switchStateChanged(); // 发出开关状态变化信号
}



void CircuitComponent::updateSwitchAppearance() {
    QGraphicsLineItem* middleLine = nullptr;

    // 查找中间的翘起线
    for (QGraphicsItem* item : childItems()) {
        if (QGraphicsLineItem* line = dynamic_cast<QGraphicsLineItem*>(item)) {
            // 根据线的位置判断是否为中间的翘起线
            // 这里假设中间的线位于固定的位置，您可能需要根据实际情况调整
            if (line->line().p1() == QPointF(5, 5) || line->line().p1() == QPointF(-5, 5)) {
                middleLine = line;
                break;
            }
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

void CircuitComponent::addWire(const QString& end, CircuitWire* wire) {
    m_wires[end].append(wire);
}

void CircuitComponent::removeWire(const QString& end) {
    if (m_wires.contains(end)) {
        for (CircuitWire* wire : m_wires[end]) {
            if (wire && wire->scene()) {
                wire->scene()->removeItem(wire);  // 确保从场景中删除导线
            }
            delete wire;  // 删除导线对象
        }
        m_wires[end].clear();  // 清除与此端点关联的导线列表
    }
}





void CircuitComponent::removeAllWires() {
    for (const QString& end : m_wires.keys()) {
        removeWire(end);
    }
    m_wires.clear();
}

void CircuitComponent::updateWires() {

    for (const QString& end : m_wires.keys()) {
        QList<CircuitWire*>& wireList = m_wires[end];
        for (CircuitWire* wire : wireList) {
            if (wire && wire->scene()) {  // 确保导线仍然存在且有效

                wire->updatePosition();
            } else {
                qDebug() << "导线不存在或无效：" << wire;
            }
        }
    }
}




QPointF CircuitComponent::getWireEndPosition(const QString& end) const {
    if (m_type == "开关") {
        if (end == "1端") {
            return mapToScene(QPointF(-10, 5)); // 左端
        } else if (end == "2端") {
            return mapToScene(QPointF(70, 5)); // 右端
        }
    } else if (m_type == "灯泡") {
        // 返回灯泡的中心位置
        return mapToScene(QPointF(0, 0));
    } else if (m_type == "电源") {
        if (end == "正端") {
            return mapToScene(QPointF(20, 0)); // 正端
        } else if (end == "负端") {
            return mapToScene(QPointF(-20, 0)); // 负端
        }
    } else {
        // 默认返回中心位置
        return mapToScene(boundingRect().center());
    }
}



void CircuitComponent::addToGroup(QGraphicsItem* item) {
    item->setAcceptedMouseButtons(Qt::NoButton); // 禁用子项的鼠标事件
    QGraphicsItemGroup::addToGroup(item);
}

void CircuitComponent::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsItemGroup::mouseMoveEvent(event);
    updateWires();  // 更新连线
    emit positionChanged();  // 发射位置变化的信号
}

QVariant CircuitComponent::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSelectedHasChanged) {
        bool selected = value.toBool();
        QPen pen = selected ? QPen(Qt::red, 2) : QPen(Qt::black, 2);
        QColor textColor = selected ? Qt::red : Qt::black;

        for (QGraphicsItem* item : childItems()) {
            if (QGraphicsEllipseItem* ellipse = dynamic_cast<QGraphicsEllipseItem*>(item)) {
                ellipse->setPen(pen);
            } else if (QGraphicsLineItem* line = dynamic_cast<QGraphicsLineItem*>(item)) {
                line->setPen(pen);
            } else if (QGraphicsPathItem* path = dynamic_cast<QGraphicsPathItem*>(item)) {
                path->setPen(pen);
            } else if (QGraphicsTextItem* text = dynamic_cast<QGraphicsTextItem*>(item)) {
                text->setDefaultTextColor(textColor);
            }
        }
    }

    if (change == ItemPositionHasChanged) {
        updateWires();  // 更新连线
        emit positionChanged();  // 发射位置变化的信号
    }

    return QGraphicsItemGroup::itemChange(change, value);
}

void CircuitComponent::paint(QPainter* painter,
                             const QStyleOptionGraphicsItem* option, QWidget* widget) {
    // 不需要自定义绘制，子项会自动绘制
}




CircuitComponent* createSwitch(int number) {
    CircuitComponent* group = new CircuitComponent(QString("开关%1").arg(number), "开关");

    // 两个小圆圈
    QGraphicsEllipseItem* circle1 = new QGraphicsEllipseItem(0, 0, 10, 10);
    circle1->setPen(QPen(Qt::black, 2));  // 设置线条宽度为 2
    QGraphicsEllipseItem* circle2 = new QGraphicsEllipseItem(50, 0, 10, 10);
    circle2->setPen(QPen(Qt::black, 2));  // 设置线条宽度为 2
    group->addToGroup(circle1);
    group->addToGroup(circle2);

    // 圆圈外两端的实线
    QGraphicsLineItem* line1 = new QGraphicsLineItem(QLineF(-10, 5, 0, 5));  // 左端实线
    line1->setPen(QPen(Qt::black, 2));  // 设置线条宽度为 2
    group->addToGroup(line1);
    QGraphicsLineItem* line2 = new QGraphicsLineItem(QLineF(60, 5, 70, 5));  // 右端实线
    line2->setPen(QPen(Qt::black, 2));  // 设置线条宽度为 2
    group->addToGroup(line2);

    // 中间的翘起实线，表示未闭合，翘起角度为 30 度
    QGraphicsLineItem* middleLine = new QGraphicsLineItem(QLineF(5, 5, 55, -10));
    middleLine->setPen(QPen(Qt::black, 2));  // 设置线条宽度为 2
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

    // 创建灯泡的空心部分，中心在 (0, 0)
    QGraphicsEllipseItem* lampBody = new QGraphicsEllipseItem(-15, -15, 30, 30); // (-15, -15, 30, 30)
    lampBody->setPen(QPen(Qt::black, 2));
    lampBody->setBrush(Qt::NoBrush);
    lampBody->setAcceptedMouseButtons(Qt::NoButton); // 禁用鼠标事件
    lampComponent->addToGroup(lampBody);

    // 创建文本标签，显示编号
    QGraphicsTextItem* lampLabel = new QGraphicsTextItem(QString(label));
    lampLabel->setPos(-5, -10); // 根据需要调整位置
    lampLabel->setAcceptedMouseButtons(Qt::NoButton); // 禁用鼠标事件
    lampComponent->addToGroup(lampLabel);

    // 定义灯泡中心和外圆半径
    const qreal radius = 15.0; // 半径 15
    const qreal lineLength = 10.0; // 每根线的长度

    // 创建六根线，分别按60度间隔排列
    for (int i = 0; i < 6; ++i) {
        // 计算每根线的角度 (60度间隔)
        qreal angle = qDegreesToRadians(60.0 * i);
        // 计算每根线的起点（灯泡外圆上的点）
        qreal startX = radius * qCos(angle);
        qreal startY = radius * qSin(angle);
        // 计算每根线的终点（起点再延伸lineLength）
        qreal endX = startX + lineLength * qCos(angle);
        qreal endY = startY + lineLength * qSin(angle);
        // 创建线条并添加到组件中
        QGraphicsLineItem* line = new QGraphicsLineItem(startX, startY, endX, endY);
        line->setPen(QPen(Qt::black, 2));
        line->setAcceptedMouseButtons(Qt::NoButton); // 禁用鼠标事件
        lampComponent->addToGroup(line);
    }

    return lampComponent;
}










// 自定义电源图形，添加编号
CircuitComponent* createPower(int number) {
    CircuitComponent* group = new CircuitComponent(QString("电源%1").arg(number), "电源");

    // 将圆圈中心放在 (0,0)
    QGraphicsEllipseItem* powerCircle = new QGraphicsEllipseItem(-20, -20, 40, 40); // (-20, -20, 40, 40)
    powerCircle->setPen(QPen(Qt::black, 2));
    powerCircle->setBrush(Qt::NoBrush);
    powerCircle->setAcceptedMouseButtons(Qt::NoButton); // 禁用鼠标事件
    group->addToGroup(powerCircle);

    // 中间的“S”形实线
    QPainterPath sShape;
    sShape.moveTo(-10, 10);
    sShape.cubicTo(-10, -20, 10, 20, 10, -10);
    QGraphicsPathItem* sItem = new QGraphicsPathItem(sShape);
    sItem->setPen(QPen(Qt::black, 2));
    sItem->setAcceptedMouseButtons(Qt::NoButton); // 禁用鼠标事件
    group->addToGroup(sItem);

    // 在电源图形左侧添加矩形框和编号
    QGraphicsRectItem* rect = new QGraphicsRectItem(-70, -12.5, 50, 25); // 调整位置以适应中心
    rect->setPen(QPen(Qt::black, 2));
    rect->setBrush(Qt::NoBrush);
    rect->setAcceptedMouseButtons(Qt::NoButton); // 禁用鼠标事件
    group->addToGroup(rect);

    QGraphicsTextItem* numberItem = new QGraphicsTextItem(QString("电源%1").arg(number));
    numberItem->setPos(-65, -10); // 根据需要调整位置
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
