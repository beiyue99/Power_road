#include "circuitscene.h"
#include "circuitcomponent.h"
#include <QGraphicsSceneMouseEvent>

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
    // 获取点击的 item
    QGraphicsItem* item = itemAt(event->scenePos(), QTransform());

    // 确保点击的是一个有效的图元
    if (item) {
        // 尝试将 item 转换为 CircuitComponent
        CircuitComponent* component = nullptr;
        // 首先检查 item 是否已经是 CircuitComponent
        if ((component = dynamic_cast<CircuitComponent*>(item))) {
        } else if (item->parentItem()) {
            component = dynamic_cast<CircuitComponent*>(item->parentItem());
        }

        // 如果找到了有效的 CircuitComponent
        if (component) {
            QString itemName = component->getName();
            QString itemType = component->getType();
            emit itemClicked(component);
        }
    }

    // 保留默认的事件处理
    QGraphicsScene::mousePressEvent(event);
}

