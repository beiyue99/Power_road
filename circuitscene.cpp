#include "circuitscene.h"
#include "circuitcomponent.h"

#include <QGraphicsSceneMouseEvent>

CircuitScene::CircuitScene(QObject* parent)
    : QGraphicsScene(parent) {
}

void CircuitScene::addComponent(QGraphicsItem* component) {
    addItem(component); // 将元件添加到场景中
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

            // 点击的是 CircuitComponent
        } else if (item->parentItem()) {
            // 检查 parentItem 是否是 CircuitComponent
            component = dynamic_cast<CircuitComponent*>(item->parentItem());
        }

        // 如果找到了有效的 CircuitComponent
        if (component) {
            QString itemName = component->getName();
            QString itemType = component->getType();
            int rotation = component->rotation(); // 获取旋转角度
            int posX = component->pos().x();
            int posY = component->pos().y();

            // 触发信号，更新右侧信息面板
            emit componentSelected(component);
            emit itemClicked(itemName, itemType, rotation, posX, posY);
        }
    }

    // 保留默认的事件处理
    QGraphicsScene::mousePressEvent(event);
}


