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










// 重载鼠标移动事件
void CircuitScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    // 使用 event 的场景位置来获取当前鼠标下的图元 (QGraphicsItem)
    QGraphicsItem* item = itemAt(event->scenePos(), QTransform());

    // 如果鼠标下确实有一个图元 (item)
    if (item) {
        CircuitComponent* component = nullptr;

        // 首先，检查 item 是否是 CircuitComponent 类型
        // dynamic_cast 用于将 QGraphicsItem 转换为 CircuitComponent，如果转换成功，则表示 item 是 CircuitComponent
        if ((component = dynamic_cast<CircuitComponent*>(item))) {
            // 如果 item 是一个 CircuitComponent，则将其记录为当前被拖拽的组件
            draggedComponent = component;
            emit componentDragged(component);  // 发送信号
        }
        // 如果 item 不是 CircuitComponent，那么我们还可以检查它是否有父图元
        else if (item->parentItem()) {
            // 尝试将 item 的父图元转换为 CircuitComponent
            component = dynamic_cast<CircuitComponent*>(item->parentItem());
            // 如果父图元也是 CircuitComponent
            if (component) {
                // 记录父图元为当前拖拽的组件
                draggedComponent = component;
                // 同样发送 componentDragged 信号，传递该组件
                emit componentDragged(component);  // 发送信号
            }
        }
    }
    // 调用父类的 mouseMoveEvent 来确保其他的默认鼠标移动行为正常处理
    QGraphicsScene::mouseMoveEvent(event);
}



// 重载鼠标释放事件
void CircuitScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    // 在鼠标释放时，清空拖拽的元件
    draggedComponent = nullptr;
    QGraphicsScene::mouseReleaseEvent(event);  // 保留默认的事件处理
}
