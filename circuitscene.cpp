#include "circuitscene.h"
#include "circuitcomponent.h"
#include <QGraphicsSceneMouseEvent>
#include "circuiteditor.h"
CircuitScene::CircuitScene(QObject* parent)
    : QGraphicsScene(parent) {
}




void CircuitScene::addComponent(CircuitComponent* component) {
    components.append(component); // 将元件添加到容器
    addItem(component); // 将元件添加到场景中
}


CircuitComponent* CircuitScene::getSelectedComponent() {
    QList<QGraphicsItem*> selectedItems = this->selectedItems();
    for (QGraphicsItem* item : selectedItems) {
        if (CircuitComponent* component = dynamic_cast<CircuitComponent*>(item)) {
            return component;
        }
    }
    return nullptr;  // 如果没有选中的CircuitComponent，则返回nullptr
}

QList<CircuitComponent *> CircuitScene::getAllComponents()
{
     return components; // 返回存储元件的容器
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
        }
    }
    QGraphicsScene::mousePressEvent(event);
}

void CircuitScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if (isDragging) {
        QGraphicsItem* item = itemAt(event->scenePos(), QTransform());
        if (item) {
            CircuitComponent* component = nullptr;
            if ((component = dynamic_cast<CircuitComponent*>(item))) {
                draggedComponent = component;
                emit componentDragged(component);
            } else if (item->parentItem()) {
                component = dynamic_cast<CircuitComponent*>(item->parentItem());
                if (component) {
                    draggedComponent = component;
                    emit componentDragged(component);
                }
            }
        }
    }
    QGraphicsScene::mouseMoveEvent(event);
}

void CircuitScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    if (isDragging) {
        isDragging = false; // 停止拖拽
        draggedComponent = nullptr;
    }
    QGraphicsScene::mouseReleaseEvent(event);
}
