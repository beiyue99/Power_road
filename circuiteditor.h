#ifndef CIRCUITEDITOR_H
#define CIRCUITEDITOR_H
#include "circuitscene.h"  // 确保这个路径是正确的

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

class CircuitScene;

class CircuitEditor : public QWidget {
    Q_OBJECT

public:
    CircuitEditor(QWidget* parent = nullptr);

public slots:
    void updateComponentDetails(CircuitComponent* component);
    void onRotationChanged();
private:
    CircuitScene* scene; // 声明 scene 成员变量

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


    QLabel* label1 = new QLabel;
    QLabel* label2 = new QLabel;

    QComboBox* comboBox1;  // 用于1端连接元件1
    QComboBox* comboBox2;  // 用于1端连接元件2
    QComboBox* comboBox3;  // 用于1端连接元件3

    QComboBox* comboBox4;  // 用于2端连接元件1
    QComboBox* comboBox5;  // 用于2端连接元件2
    QComboBox* comboBox6;  // 用于2端连接元件3
};

#endif // CIRCUITEDITOR_H
