#include "circuitscene.h"
#include "circuitcomponent.h"
#include <QGraphicsSceneMouseEvent>

CircuitScene::CircuitScene(QObject* parent)
    : QGraphicsScene(parent) {
}


void CircuitScene::updateWires() {
    for (CircuitComponent* component : components) {
        component->updateWires();
    }
    updatePowerStatus();
}

void CircuitScene::addComponent(CircuitComponent* component) {
    components.append(component);
    addItem(component);

    connect(component, &CircuitComponent::positionChanged, this, [this, component]() {
        emit componentMoved(component);
        updateWires();  // 每当组件移动时，更新所有连线
        updatePowerStatus(); // 调用供电状态更新
    });

    connect(component, &CircuitComponent::switchStateChanged, this, [this, component]() {
        updatePowerStatus(); // 开关状态变化时更新供电状态
    });

    updatePowerStatus(); // 添加组件后调用
}

void CircuitScene::removeComponentAndWires(CircuitComponent* component) {
    // 断开所有与该组件相关的信号槽
    disconnect(component, nullptr, this, nullptr);

    // 记录待删除的导线
    QList<CircuitWire*> wiresToRemove;

    // 遍历所有元件，找到与当前元件相连的所有导线
    for (CircuitComponent* otherComponent : components) {
        for (const QString& end : otherComponent->getWires().keys()) {
            QList<CircuitWire*>& wireList = otherComponent->getWires()[end];
            for (CircuitWire* wire : wireList) {
                // 检查导线是否与要删除的元件相连
                if (wire->getStartItem() == component || wire->getEndItem() == component) {
                    wiresToRemove.append(wire); // 记录待删除的导线
                    otherComponent->removeWire(end); // 清除其他元件中对此导线的引用
                }
            }
        }
    }

    // 删除所有与该元件相关的导线
    for (CircuitWire* wire : wiresToRemove) {
        if (wire->scene() == this) {
            removeItem(wire); // 从场景中移除导线
            delete wire;      // 释放导线内存
        }
    }

    // 从组件列表中移除该元件
    components.removeOne(component);
    removeItem(component); // 从场景中移除元件
    delete component;      // 删除元件

    updatePowerStatus(); // 更新电路状态
}






CircuitComponent* CircuitScene::getSelectedComponent() {
    QList<QGraphicsItem*> selectedItems = this->selectedItems();
    for (QGraphicsItem* item : selectedItems) {
        if (CircuitComponent* component = dynamic_cast<CircuitComponent*>(item)) {
            return component;
        }
        else if (item->parentItem()) {
            if (CircuitComponent* parentComponent = dynamic_cast<CircuitComponent*>(item->parentItem())) {
                return parentComponent;
            }
        }
    }
    return nullptr;
}

QList<CircuitComponent *> CircuitScene::getAllComponents() {
    return components;
}

void CircuitScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsItem* item = itemAt(event->scenePos(), QTransform());
    if (item) {
        CircuitComponent* component = nullptr;
        if ((component = dynamic_cast<CircuitComponent*>(item))) {
        }
        else if (item->parentItem()) {
            component = dynamic_cast<CircuitComponent*>(item->parentItem());
        }

        if (component) {
            emit itemClicked(component);
            isDragging = true; // 开始拖拽
        }
        else {
            emit noItemClicked(); // 发送没有点击任何元件的信号
        }
    }
    else {
        emit noItemClicked(); // 发送没有点击任何元件的信号
    }
    QGraphicsScene::mousePressEvent(event);
}

// circuitscene.cpp

void CircuitScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsScene::mouseMoveEvent(event);
    // 不需要在这里调用 updateWires()，因为已经在 CircuitComponent::itemChange 中处理
}



void CircuitScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    if (isDragging) {
        // 停止拖拽
        isDragging = false;
        // 更新所有连线
        for (CircuitComponent* component : components) {
            component->updateWires();
        }
    }
    QGraphicsScene::mouseReleaseEvent(event);
}





void CircuitScene::updatePowerStatus() {
    // 清除所有导线的颜色
    QList<QGraphicsItem*> allItems = items();
    QList<CircuitWire*> allWires;
    QList<CircuitComponent*> components;

    for (QGraphicsItem* item : allItems) {
        if (CircuitWire* wire = dynamic_cast<CircuitWire*>(item)) {
            wire->setPen(QPen(Qt::black, 2)); // 导线无电
            wire->update();
            allWires.append(wire);
        } else if (CircuitComponent* component = dynamic_cast<CircuitComponent*>(item)) {
            components.append(component);
            // 如果是灯泡，熄灭它
            if (component->getType() == "灯泡") {
                for (QGraphicsItem* childItem : component->childItems()) {
                    if (QGraphicsEllipseItem* ellipse = dynamic_cast<QGraphicsEllipseItem*>(childItem)) {
                        ellipse->setBrush(Qt::NoBrush); // 灯泡熄灭
                    }
                }
            }
        }
    }

    // 构建端点的邻接表
    QMap<QString, QList<QString>> adjacencyMap; // 端点名称到连接端点列表的映射
    QMap<QString, CircuitComponent*> endToComponentMap; // 端点名称到组件的映射

    // 为每个组件的端点创建唯一的名称
    auto getEndPointName = [](CircuitComponent* component, const QString& end) {
        return component->getName() + "-" + end;
    };

    // 初始化邻接表
    for (CircuitComponent* component : components) {
        QStringList ends;
        if (component->getType() == "开关") {
            ends << "1端" << "2端";
        } else if (component->getType() == "电源" || component->getType() == "灯泡") {
            ends << "端"; // 使用 "端" 作为唯一的端点名称
        }

        for (const QString& end : ends) {
            QString endPointName = getEndPointName(component, end);
            endToComponentMap[endPointName] = component;
            adjacencyMap[endPointName] = QList<QString>();
        }
    }

    // 添加导线连接到邻接表
    for (CircuitWire* wire : allWires) {
        CircuitComponent* startComponent = dynamic_cast<CircuitComponent*>(wire->getStartItem());
        CircuitComponent* endComponent = dynamic_cast<CircuitComponent*>(wire->getEndItem());

        QString startEnd = wire->getStartEndType();
        QString endEnd = wire->getEndEndType();

        // 对于只有一个端点的组件，确保端点名称与之前一致
        if (startComponent->getType() != "开关") {
            startEnd = "端";
        }
        if (endComponent->getType() != "开关") {
            endEnd = "端";
        }

        QString startEndPoint = getEndPointName(startComponent, startEnd);
        QString endEndPoint = getEndPointName(endComponent, endEnd);

        // 添加连接
        adjacencyMap[startEndPoint].append(endEndPoint);
        adjacencyMap[endEndPoint].append(startEndPoint);
    }

    // BFS 遍历，传播电流状态
    QSet<QString> visited;
    QQueue<QString> queue;

    // 初始化队列，添加所有电源的端点
    for (CircuitComponent* component : components) {
        if (component->getType() == "电源") {
            QString endPointName = getEndPointName(component, "端");
            queue.enqueue(endPointName);
            visited.insert(endPointName);
        }
    }

    while (!queue.isEmpty()) {
        QString currentEndPoint = queue.dequeue();
        CircuitComponent* currentComponent = endToComponentMap[currentEndPoint];

        // 获取当前端点的连接列表
        QList<QString> neighbors = adjacencyMap[currentEndPoint];

        // 检查开关的内部连接
        if (currentComponent->getType() == "开关") {
            QString currentEnd = currentEndPoint.section('-', -1); // 获取端点名称（"1端" 或 "2端"）
            QString otherEnd = (currentEnd == "1端") ? "2端" : "1端";
            QString otherEndPoint = getEndPointName(currentComponent, otherEnd);

            if (currentComponent->isClosed()) {
                // 开关闭合，内部端点相连
                neighbors.append(otherEndPoint);
            }
        }

        for (const QString& neighborEndPoint : neighbors) {
            if (!visited.contains(neighborEndPoint)) {
                visited.insert(neighborEndPoint);
                queue.enqueue(neighborEndPoint);

                CircuitComponent* neighborComponent = endToComponentMap[neighborEndPoint];

                // 更新连接当前端点和邻居端点的导线颜色为红色
                for (CircuitWire* wire : allWires) {
                    CircuitComponent* wireStartComponent = dynamic_cast<CircuitComponent*>(wire->getStartItem());
                    CircuitComponent* wireEndComponent = dynamic_cast<CircuitComponent*>(wire->getEndItem());

                    QString wireStartEnd = wire->getStartEndType();
                    QString wireEndEnd = wire->getEndEndType();

                    // 对于只有一个端点的组件，确保端点名称与之前一致
                    if (wireStartComponent->getType() != "开关") {
                        wireStartEnd = "端";
                    }
                    if (wireEndComponent->getType() != "开关") {
                        wireEndEnd = "端";
                    }

                    QString wireStartEndPoint = getEndPointName(wireStartComponent, wireStartEnd);
                    QString wireEndEndPoint = getEndPointName(wireEndComponent, wireEndEnd);

                    if ((wireStartEndPoint == currentEndPoint && wireEndEndPoint == neighborEndPoint) ||
                        (wireStartEndPoint == neighborEndPoint && wireEndEndPoint == currentEndPoint)) {
                        wire->setPen(QPen(Qt::red, 2)); // 导线带电
                        wire->update();
                    }
                }

                // 如果邻居端点属于灯泡，点亮它
                if (neighborComponent->getType() == "灯泡") {
                    for (QGraphicsItem* item : neighborComponent->childItems()) {
                        if (QGraphicsEllipseItem* ellipse = dynamic_cast<QGraphicsEllipseItem*>(item)) {
                            ellipse->setBrush(Qt::yellow); // 灯泡发光
                        }
                    }
                }
            }
        }
    }
}

