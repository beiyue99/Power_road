#ifndef CIRCUITSCENE_H
#define CIRCUITSCENE_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QList>

class CircuitScene : public QGraphicsScene {
    Q_OBJECT

public:
    explicit CircuitScene(QObject* parent = nullptr);
    void addComponent(QGraphicsItem* component); // 添加元件到场景

};

#endif // CIRCUITSCENE_H
