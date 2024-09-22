#ifndef DLEDITCLASS_H
#define DLEDITCLASS_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QGraphicsView>
#include <QFrame>
#include <QGridLayout>
#include <QSpacerItem>

class DLEditClass : public QWidget {
    Q_OBJECT

public:
    DLEditClass(QWidget *parent = nullptr);

private:
    QLineEdit* rotationEdit;
    QLineEdit* positionXEdit;
    QLineEdit* positionYEdit;
};

#endif // DLEDITCLASS_H
