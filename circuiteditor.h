#ifndef CIRCUITEDITOR_H
#define CIRCUITEDITOR_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QFormLayout>
#include <QFrame>
#include <QStringList>
#include "clickablegraphicsview.h"
#include "componentfactory.h"


class CircuitEditor : public QWidget {
    Q_OBJECT

public:
    CircuitEditor(QWidget* parent = nullptr);

private:
    int powerCounter = 1;   // 电源编号
    int switchCounter = 1;  // 开关编号
    char lampCounter = 'A'; // 灯泡编号
    QVBoxLayout* detailLayout; // 详细信息布局

};

#endif // CIRCUITEDITOR_H
