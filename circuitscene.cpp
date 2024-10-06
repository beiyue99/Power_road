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
        detectAndHighlightCycles(); // 调用回路检测
    });

    detectAndHighlightCycles(); // 添加组件后调用
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

void CircuitScene::detectAndHighlightCycles() {
    // 清除之前的高亮设置
    qDebug() << "Clearing previous highlights.";
    for (CircuitComponent* component : components) {
        if (component->getType() == "灯泡") {
            for (QGraphicsItem* item : component->childItems()) {
                if (QGraphicsEllipseItem* ellipse = dynamic_cast<QGraphicsEllipseItem*>(item)) {
                    ellipse->setBrush(QBrush(Qt::NoBrush));
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

    // 构建图的邻接表
    QMap<CircuitComponent*, QList<CircuitComponent*>> adjacencyMap;

    // 初始化邻接表
    for (CircuitComponent* component : components) {
        adjacencyMap[component] = QList<CircuitComponent*>();
    }

    // 遍历所有连线，填充邻接表
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
    qDebug() << "Total unique wires:" << allWires.size();

    // 填充邻接表，考虑开关状态
    for (CircuitWire* wire : allWires) {
        CircuitComponent* startComponent = dynamic_cast<CircuitComponent*>(wire->getStartItem());
        CircuitComponent* endComponent = dynamic_cast<CircuitComponent*>(wire->getEndItem());

        if (!startComponent || !endComponent) {
            qDebug() << "Wire has invalid start or end component.";
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
            qDebug() << "Active connection between:" << startComponent->getName() << "and" << endComponent->getName();
        }
    }

    // 使用DFS检测所有回路
    QSet<QList<CircuitComponent*>> cycles;

    std::function<void(CircuitComponent*, CircuitComponent*, QList<CircuitComponent*>&)> dfs =
        [&](CircuitComponent* current, CircuitComponent* parent, QList<CircuitComponent*>& path) {
            path.append(current);
            qDebug() << "Visiting component:" << current->getName();

            for (CircuitComponent* neighbor : adjacencyMap[current]) {
                if (neighbor == parent) continue;
                if (path.contains(neighbor)) {
                    // 发现一个回路
                    int index = path.indexOf(neighbor);
                    QList<CircuitComponent*> cycle = path.mid(index);
                    cycle.append(neighbor);
                    qDebug() << "Cycle detected involving components:";
                    for (CircuitComponent* comp : cycle) {
                        qDebug() << comp->getName();
                    }
                    cycles.insert(cycle);
                } else if (!path.contains(neighbor)) {
                    dfs(neighbor, current, path);
                }
            }

            path.removeLast();
        };

    for (CircuitComponent* component : components) {
        QList<CircuitComponent*> path;
        dfs(component, nullptr, path);
    }

    // 输出调试信息
    qDebug() << "Detected cycles:";
    for (const QList<CircuitComponent*>& cycle : cycles) {
        QString cycleStr;
        for (CircuitComponent* comp : cycle) {
            cycleStr += comp->getName() + " -> ";
        }
        cycleStr.chop(4); // 移除最后的箭头
        qDebug() << cycleStr;
    }

    // 收集回路中的连线和灯泡
    QSet<CircuitWire*> wiresInCycles;
    QSet<CircuitComponent*> bulbsInCycles;

    for (const QList<CircuitComponent*>& cycle : cycles) {
        // 遍历回路中的组件，收集连线
        for (int i = 0; i < cycle.size(); ++i) {
            CircuitComponent* current = cycle[i];
            CircuitComponent* next = cycle[(i + 1) % cycle.size()]; // 使用取余操作来形成闭合环

            // 查找连接 current 和 next 的连线
            for (auto it = current->getWires().begin(); it != current->getWires().end(); ++it) {
                for (CircuitWire* wire : it.value()) {
                    CircuitComponent* other = (wire->getStartItem() == current) ? dynamic_cast<CircuitComponent*>(wire->getEndItem()) : dynamic_cast<CircuitComponent*>(wire->getStartItem());
                    if (other == next) {
                        wiresInCycles.insert(wire);
                    }
                }
            }
        }

        // 收集回路中的灯泡
        for (CircuitComponent* component : cycle) {
            if (component->getType() == "灯泡") {
                bulbsInCycles.insert(component);
            }
        }
    }

    // 高亮显示回路中的连线和灯泡
    qDebug() << "Wires in cycles:";
    for (CircuitWire* wire : wiresInCycles) {
        CircuitComponent* startComponent = dynamic_cast<CircuitComponent*>(wire->getStartItem());
        CircuitComponent* endComponent = dynamic_cast<CircuitComponent*>(wire->getEndItem());
        if (startComponent && endComponent) {
            qDebug() << "Wire between" << startComponent->getName() << "and" << endComponent->getName();
            wire->setPen(QPen(Qt::red, 2));
            wire->update();
        }
    }

    qDebug() << "Bulbs in cycles:";
    for (CircuitComponent* bulb : bulbsInCycles) {
        qDebug() << bulb->getName();
        for (QGraphicsItem* item : bulb->childItems()) {
            if (QGraphicsEllipseItem* ellipse = dynamic_cast<QGraphicsEllipseItem*>(item)) {
                ellipse->setBrush(QBrush(Qt::yellow));
            }
        }
    }
}

