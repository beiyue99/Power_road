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
#include "circuitcomponent.h"

class CircuitEditor : public QWidget {
    Q_OBJECT

public:
    CircuitEditor(QWidget* parent = nullptr);

public slots:
    void updateComponentDetails(CircuitComponent* component);
private:
    int powerCounter = 1;   // 电源编号
    int switchCounter = 1;  // 开关编号
    char lampCounter = 'A'; // 灯泡编号
    QLabel* nameLabel;
    QLineEdit* rotationEdit;
    QLineEdit* posXEdit;
    QLineEdit* posYEdit;
    QLabel* typeLabel;
    QPushButton* disconnectButton;
    QPushButton* connectButton;
    ClickableGraphicsView* lampView;
    ClickableGraphicsView* switchView;
    ClickableGraphicsView* powerView;

    QComboBox* comboBox1;  // ComboBox1 用于连接元件1
    QComboBox* comboBox2;  // ComboBox2 用于连接元件2

};

#endif // CIRCUITEDITOR_H
