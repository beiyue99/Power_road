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
    void updateWires() ;
signals:
    void itemClicked(CircuitComponent* component);
    void componentDragged(CircuitComponent* component);  // 新增信号
    void noItemClicked(); // 新增信号，用于表示没有点击任何元件
    void componentMoved(CircuitComponent* component);

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
     void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
     void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
private:
     QList<CircuitComponent*> components; // 存储场景中所有元件的容器
      CircuitComponent* draggedComponent = nullptr;  // 当前拖拽的元件
      bool isDragging = false; // 新增标志，追踪是否正在拖拽
};

#endif // CIRCUITSCENE_H
