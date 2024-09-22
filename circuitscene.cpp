#include "circuitscene.h"



CircuitScene::CircuitScene(QObject* parent)
    : QGraphicsScene(parent) {
}

void CircuitScene::addComponent(QGraphicsItem* component) {
    addItem(component); // 将元件添加到场景中
}

