#ifndef CIRCUITSCENE_H
#define CIRCUITSCENE_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QList>
#include "circuitcomponent.h"
#include "circuiteditor.h"
class CircuitScene : public QGraphicsScene {
    Q_OBJECT

public:
    explicit CircuitScene(QObject* parent = nullptr);
    void addComponent(QGraphicsItem* component); // 添加元件到场景

signals:
    void itemClicked(const QString& itemName, const QString& itemType, int rotation, int posX, int posY);
    void componentSelected(CircuitComponent* component);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
};

#endif // CIRCUITSCENE_H
