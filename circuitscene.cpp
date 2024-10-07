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
    updateCircuitStatus(); // 更新回路状态
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
    qDebug() << "正在删除元件：" << component->getName() << "类型：" << component->getType();

    // 先断开所有与该组件相关的信号槽
    disconnect(component, nullptr, this, nullptr);

    // 遍历所有元件，找到与当前元件相连的所有导线
    QList<CircuitWire*> wiresToRemove;

    for (CircuitComponent* otherComponent : components) {
        for (const QString& end : otherComponent->getWires().keys()) {
            QList<CircuitWire*>& wireList = otherComponent->getWires()[end];
            for (CircuitWire* wire : wireList) {
                if (wire->getStartItem() == component || wire->getEndItem() == component) {
                    qDebug() << "找到与元件相连的导线：" << wire;
                    wiresToRemove.append(wire); // 记录待删除的导线

                    // 在其他元件中移除对该导线的引用
                    otherComponent->removeWire(end); // 重要：清除引用
                }
            }
        }
    }

    // 删除所有与该元件相关的导线
    for (CircuitWire* wire : wiresToRemove) {
        if (wire->scene() == this) { // 确保该导线属于当前场景
            qDebug() << "删除导线：" << wire;
            removeItem(wire); // 从场景中删除导线
            delete wire;      // 释放导线内存
        } else {
            qDebug() << "导线不在当前场景中，跳过删除：" << wire;
        }
    }

    // 最后从组件列表中移除该元件，并删除它
    components.removeOne(component);
    removeItem(component);
    qDebug() << "删除元件：" << component->getName();
    delete component;
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

// 新增回路检测与高亮功能
void CircuitScene::updateCircuitStatus() {
    // Step 1: Clear previous highlights
    for (CircuitComponent* component : getAllComponents()) {
        if (component->getType() == "灯泡") {
            for (QGraphicsItem* item : component->childItems()) {
                if (QGraphicsEllipseItem* ellipse = dynamic_cast<QGraphicsEllipseItem*>(item)) {
                    ellipse->setBrush(Qt::NoBrush); // 重置为无填充
                }
            }
        }
        // 重置连线颜色
        for (QString end : component->m_wires.keys()) {
            for (CircuitWire* wire : component->m_wires[end]) {
                wire->setPen(QPen(Qt::black, 2)); // 重置为黑色
            }
        }
    }

    // Step 2: 构建邻接表，考虑闭合的开关
    QMap<CircuitComponent*, QList<CircuitComponent*>> adjacencyMap;

    for (CircuitComponent* component : getAllComponents()) {
        for (QString end : component->m_wires.keys()) {
            for (CircuitWire* wire : component->m_wires[end]) {
                CircuitComponent* otherComponent = (wire->m_startItem == component) ? dynamic_cast<CircuitComponent*>(wire->m_endItem) : dynamic_cast<CircuitComponent*>(wire->m_startItem);
                if (otherComponent) {
                    bool connectionValid = true;
                    // 如果当前组件是开关且断开，连接无效
                    if (component->getType() == "开关" && !component->isClosed()) {
                        connectionValid = false;
                    }
                    // 如果连接的组件是开关且断开，连接无效
                    if (otherComponent->getType() == "开关" && !otherComponent->isClosed()) {
                        connectionValid = false;
                    }
                    if (connectionValid) {
                        adjacencyMap[component].append(otherComponent);
                    }
                }
            }
        }
    }

    // Step 3: 使用DFS寻找所有闭合回路
    QSet<QList<CircuitComponent*>> cycles;

    QSet<CircuitComponent*> visited;

    // 定义DFS函数
    std::function<void(CircuitComponent*, CircuitComponent*, QList<CircuitComponent*>&)> dfs = [&](CircuitComponent* current, CircuitComponent* parent, QList<CircuitComponent*>& path) {
        visited.insert(current);
        path.append(current);

        for (CircuitComponent* neighbor : adjacencyMap[current]) {
            if (neighbor == parent) continue;
            if (path.contains(neighbor)) {
                // 发现一个回路
                int index = path.indexOf(neighbor);
                QList<CircuitComponent*> cycle = path.mid(index);
                cycle.append(neighbor);
                // 排序回路以避免重复
                QList<CircuitComponent*> sortedCycle = cycle;
                std::sort(sortedCycle.begin(), sortedCycle.end(), [](CircuitComponent* a, CircuitComponent* b) -> bool {
                    return a->getName() < b->getName();
                });
                cycles.insert(sortedCycle);
            }
            else if (!visited.contains(neighbor)) {
                dfs(neighbor, current, path);
            }
        }

        path.removeLast();
    };

    // 从所有电源开始DFS
    for (CircuitComponent* component : getAllComponents()) {
        if (component->getType() == "电源") {
            QList<CircuitComponent*> path;
            dfs(component, nullptr, path);
        }
    }

    // Step 4: 高亮闭合回路中的连线和灯泡
    for (const QList<CircuitComponent*>& cycle : cycles) {
        // 检查回路中所有开关是否都闭合
        bool isClosed = true;
        for (CircuitComponent* comp : cycle) {
            if (comp->getType() == "开关" && !comp->isClosed()) {
                isClosed = false;
                break;
            }
        }
        if (isClosed) {
            // 高亮回路中的连线
            for (int i = 0; i < cycle.size() -1; ++i) {
                CircuitComponent* a = cycle[i];
                CircuitComponent* b = cycle[i+1];
                // 查找连接a和b的连线
                for (QString end : a->m_wires.keys()) {
                    for (CircuitWire* wire : a->m_wires[end]) {
                        CircuitComponent* other = (wire->m_startItem == a) ? dynamic_cast<CircuitComponent*>(wire->m_endItem) : dynamic_cast<CircuitComponent*>(wire->m_startItem);
                        if (other == b) {
                            wire->setPen(QPen(Qt::red, 2)); // 设置为红色
                        }
                    }
                }
            }
            // 高亮回路中的灯泡
            for (CircuitComponent* comp : cycle) {
                if (comp->getType() == "灯泡") {
                    for (QGraphicsItem* item : comp->childItems()) {
                        if (QGraphicsEllipseItem* ellipse = dynamic_cast<QGraphicsEllipseItem*>(item)) {
                            ellipse->setBrush(Qt::yellow); // 设置为黄色
                        }
                    }
                }
            }
        }
    }
}

void CircuitScene::updatePowerStatus() {
    // Step 1: 清除之前的供电状态
    for (CircuitComponent* component : components) {
        if (component->getType() == "灯泡") {
            for (QGraphicsItem* item : component->childItems()) {
                if (QGraphicsEllipseItem* ellipse = dynamic_cast<QGraphicsEllipseItem*>(item)) {
                    ellipse->setBrush(Qt::NoBrush); // 重置为无填充
                }
            }
        }
        // 重置连线颜色
        for (auto it = component->getWires().begin(); it != component->getWires().end(); ++it) {
            for (CircuitWire* wire : it.value()) {
                wire->setPen(QPen(Qt::black, 2));
                wire->update();
            }
        }
    }

    // Step 2: 构建邻接表，考虑闭合的开关
    QMap<CircuitComponent*, QList<CircuitComponent*>> adjacencyMap;

    for (CircuitComponent* component : components) {
        adjacencyMap[component] = QList<CircuitComponent*>();
    }

    QList<CircuitWire*> allWires;
    for (CircuitComponent* component : components) {
        for (auto it = component->getWires().begin(); it != component->getWires().end(); ++it) {
            for (CircuitWire* wire : it.value()) {
                allWires.append(wire);
            }
        }
    }

    // 去重连线
    allWires = allWires.toSet().toList();

    // 填充邻接表，考虑开关状态
    for (CircuitWire* wire : allWires) {
        CircuitComponent* startComponent = dynamic_cast<CircuitComponent*>(wire->getStartItem());
        CircuitComponent* endComponent = dynamic_cast<CircuitComponent*>(wire->getEndItem());

        if (!startComponent || !endComponent) {
            continue;
        }

        // 判断连线是否处于活动状态（开关闭合）
        bool isActive = true;
        if (startComponent->getType() == "开关" && !startComponent->isClosed()) {
            isActive = false;
        }
        if (endComponent->getType() == "开关" && !endComponent->isClosed()) {
            isActive = false;
        }

        if (isActive) {
            adjacencyMap[startComponent].append(endComponent);
            adjacencyMap[endComponent].append(startComponent);
        }
    }

    // Step 3: 从所有电源出发，进行 BFS 遍历
    QSet<CircuitComponent*> poweredComponents;
    QQueue<CircuitComponent*> queue;

    // 初始化队列，添加所有电源
    for (CircuitComponent* component : components) {
        if (component->getType() == "电源") {
            queue.enqueue(component);
            poweredComponents.insert(component);
        }
    }

    // BFS 遍历
    while (!queue.isEmpty()) {
        CircuitComponent* current = queue.dequeue();

        for (CircuitComponent* neighbor : adjacencyMap[current]) {
            if (!poweredComponents.contains(neighbor)) {
                poweredComponents.insert(neighbor);
                queue.enqueue(neighbor);
            }
        }
    }

    // Step 4: 高亮供电路径中的连线和灯泡
    for (CircuitComponent* component : poweredComponents) {
        if (component->getType() == "灯泡") {
            // 点亮灯泡
            for (QGraphicsItem* item : component->childItems()) {
                if (QGraphicsEllipseItem* ellipse = dynamic_cast<QGraphicsEllipseItem*>(item)) {
                    ellipse->setBrush(Qt::yellow); // 设置为黄色
                }
            }
        }

        // 高亮供电连线
        for (auto it = component->getWires().begin(); it != component->getWires().end(); ++it) {
            for (CircuitWire* wire : it.value()) {
                CircuitComponent* otherComponent = (wire->getStartItem() == component) ?
                    dynamic_cast<CircuitComponent*>(wire->getEndItem()) :
                    dynamic_cast<CircuitComponent*>(wire->getStartItem());
                if (otherComponent && poweredComponents.contains(otherComponent)) {
                    wire->setPen(QPen(Qt::red, 2)); // 设置为红色
                    wire->update();
                }
            }
        }
    }
}
