#ifndef ELECTRONICCOMPONENT_H
#define ELECTRONICCOMPONENT_H
#include <QGraphicsItem>


class ElectronicComponent : public QGraphicsItem {
public:
    ElectronicComponent(const QString &name, const QString &type, int x, int y, int rotation)
        : name(name), type(type), x(x), y(y), rotation(rotation) {}

    QString getName() const { return name; }
    QString getType() const { return type; }
    int getX() const { return x; }
    int getY() const { return y; }
    int getRotation() const { return rotation; }

    void setRotation(int rot) { rotation = rot; }
    void setX(int newX) { x = newX; }
    void setY(int newY) { y = newY; }
    virtual QStringList getConnectedComponents() const {
        return QStringList();  // 默认返回空列表，可以根据需要重写
    }
protected:
    QString name;
    QString type;
    int x, y;
    int rotation;
};


#endif // ELECTRONICCOMPONENT_H
