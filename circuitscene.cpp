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
}

void CircuitScene::addComponent(CircuitComponent* component) {
    components.append(component);
    addItem(component);

    connect(component, &CircuitComponent::positionChanged, this, [this, component]() {
        emit componentMoved(component);
    });
}

CircuitComponent* CircuitScene::getSelectedComponent() {
    QList<QGraphicsItem*> selectedItems = this->selectedItems();
    for (QGraphicsItem* item : selectedItems) {
        if (CircuitComponent* component = dynamic_cast<CircuitComponent*>(item)) {
            return component;
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
        } else if (item->parentItem()) {
            component = dynamic_cast<CircuitComponent*>(item->parentItem());
        }

        if (component) {
            emit itemClicked(component);
            isDragging = true; // 开始拖拽
        } else {
            emit noItemClicked(); // 发送没有点击任何元件的信号
        }
    } else {
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
