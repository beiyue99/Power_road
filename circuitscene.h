#ifndef CIRCUITSCENE_H
#define CIRCUITSCENE_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QList>
#include "circuitcomponent.h"
#include "circuiteditor.h"
#include <QtDebug>
class CircuitScene : public QGraphicsScene {
    Q_OBJECT

public:
    explicit CircuitScene(QObject* parent = nullptr);
    void addComponent(CircuitComponent* component); // 添加元件到场景
    CircuitComponent* getSelectedComponent() ;  //返回当前被选择的元件组
    QList<CircuitComponent*> getAllComponents(); // 声明获取所有元件的方法
signals:
    void itemClicked(CircuitComponent* component);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:
     QList<CircuitComponent*> components; // 存储场景中所有元件的容器
};

#endif // CIRCUITSCENE_H
