#include "circuiteditor.h"
#include "circuitcomponent.h"
#include "circuitscene.h"  // 添加这一行
#include <QFrame>

CircuitEditor::CircuitEditor(QWidget *parent) : QWidget(parent), powerCounter(1), switchCounter(1), lampCounter('A') {
    // 创建主布局：水平布局
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    // 左侧布局：电路元件按钮，图形在上，名字在下
    QVBoxLayout* leftLayout = new QVBoxLayout();
    // 灯泡部分
    QVBoxLayout* lampLayout = new QVBoxLayout();
    QLabel* lampLabel = new QLabel("灯泡");
    lampView = new ClickableGraphicsView();
    QGraphicsScene* lampScene = new QGraphicsScene();
    lampScene->addItem(createLampWithoutLabel());
    lampView->setScene(lampScene);
    lampView->setFrameShape(QFrame::NoFrame);
    lampLayout->addWidget(lampView);
    lampLayout->addWidget(lampLabel);

    // 开关部分
    QVBoxLayout* switchLayout = new QVBoxLayout();
    QLabel* switchLabel = new QLabel("开关");
    switchView = new ClickableGraphicsView();
    QGraphicsScene* switchScene = new QGraphicsScene();
    switchScene->addItem(createSwitchWithoutLabel());
    switchView->setScene(switchScene);
    switchView->setFrameShape(QFrame::NoFrame);
    switchLayout->addWidget(switchView);
    switchLayout->addWidget(switchLabel);

    // 电源部分
    QVBoxLayout* powerLayout = new QVBoxLayout();
    QLabel* powerLabel = new QLabel("电源");
    powerView = new ClickableGraphicsView();
    QGraphicsScene* powerScene = new QGraphicsScene();
    powerScene->addItem(createPowerWithoutLabel());
    powerView->setScene(powerScene);
    powerView->setFrameShape(QFrame::NoFrame);
    powerLayout->addWidget(powerView);
    powerLayout->addWidget(powerLabel);

    // 将灯泡、开关、电源布局添加到左侧布局
    leftLayout->addLayout(lampLayout);
    leftLayout->addLayout(switchLayout);
    leftLayout->addLayout(powerLayout);
    leftLayout->addStretch(); // 空间填充

    // 中间布局：电路编辑区域
    QGraphicsView* view = new QGraphicsView(this);
    CircuitScene* scene = new CircuitScene(this);
    view->setScene(scene);

    // 右侧布局：元件详细信息
    QVBoxLayout* rightLayout = new QVBoxLayout();

    // 第一行：元件名称
    nameLabel = new QLabel("元件名称");
    nameLabel->setStyleSheet("font-weight: bold;");
    rightLayout->addWidget(nameLabel);

    // 第二行：旋转角度
    QHBoxLayout* rotationLayout = new QHBoxLayout();
    rotationLayout->addWidget(new QLabel("旋转角:"));
    rotationEdit = new QLineEdit();
    rotationLayout->addWidget(rotationEdit);
    rightLayout->addLayout(rotationLayout);

    // 第三行：位置X
    QHBoxLayout* posXLayout = new QHBoxLayout();
    posXLayout->addWidget(new QLabel("位置X:"));
    posXEdit = new QLineEdit();
    posXLayout->addWidget(posXEdit);
    rightLayout->addLayout(posXLayout);

    // 第四行：位置Y
    QHBoxLayout* posYLayout = new QHBoxLayout();
    posYLayout->addWidget(new QLabel("位置Y:"));
    posYEdit = new QLineEdit();
    posYLayout->addWidget(posYEdit);
    rightLayout->addLayout(posYLayout);

    // 创建一个水平分隔线
    QFrame* separator2 = new QFrame();
    separator2->setFrameShape(QFrame::HLine);
    separator2->setFrameShadow(QFrame::Sunken); // 可选，设置阴影效果
    rightLayout->addWidget(separator2);

    // 类型信息
    typeLabel = new QLabel("类型：");
    typeLabel->setStyleSheet("font-weight: bold;");
    rightLayout->addWidget(typeLabel);

    // 开关状态控制按钮
    QHBoxLayout* switchControlLayout = new QHBoxLayout();
    disconnectButton = new QPushButton("断开");
    connectButton = new QPushButton("闭合");
    disconnectButton->setVisible(false); // 初始隐藏
    connectButton->setVisible(false); // 初始隐藏
    switchControlLayout->addWidget(disconnectButton);
    switchControlLayout->addWidget(connectButton);
    rightLayout->addLayout(switchControlLayout);

    // 水平分隔线
    QFrame* separator = new QFrame();
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken); // 可选的，设置阴影效果
    rightLayout->addWidget(separator);


    // 连接元件信息
    comboBox1 = new QComboBox();
    comboBox1->addItems({"元件1", "元件2", "元件3"});
    comboBox1->setVisible(false); // 初始隐藏

    comboBox2 = new QComboBox();
    comboBox2->addItems({"元件1", "元件2", "元件3"});
    comboBox2->setVisible(false); // 初始隐藏


    rightLayout->addWidget(comboBox1);
    rightLayout->addWidget(comboBox2);


    // 将布局加入主布局中
    mainLayout->addLayout(leftLayout, 1); // 左侧电路元件按钮布局
    mainLayout->addWidget(view, 4);       // 中间电路编辑区
    mainLayout->addLayout(rightLayout, 1); // 右侧元件详细信息布局

    // 点击事件：添加带编号的元件到中间电路区域
    connect(lampView, &ClickableGraphicsView::clicked, [this, scene]() {
        scene->addItem(createLamp(lampCounter++));
    });

    connect(switchView, &ClickableGraphicsView::clicked, [this, scene]() {
        scene->addItem(createSwitch(switchCounter++));
    });

    connect(powerView, &ClickableGraphicsView::clicked, [this, scene]() {
        scene->addItem(createPower(powerCounter++));
    });

    connect(scene, &CircuitScene::itemClicked, this, &CircuitEditor::updateComponentDetails);



}


void CircuitEditor::updateComponentDetails(CircuitComponent* component) {
    nameLabel->setText(component->getName());
    rotationEdit->setText(QString::number(component->rotation()));
    posXEdit->setText(QString::number(component->pos().x()));
    posYEdit->setText(QString::number(component->pos().y()));
    typeLabel->setText(component->getType());

    // 根据类型显示或隐藏按钮和 comboBox
    if (component->getType() == "开关") {
        disconnectButton->setVisible(true);
        connectButton->setVisible(true);
        comboBox1->setVisible(true);
        comboBox2->setVisible(true);

    } else {
        // 如果不是开关，隐藏这些按钮和 comboBox
        disconnectButton->setVisible(false);
        connectButton->setVisible(false);
        comboBox1->setVisible(false);
        comboBox2->setVisible(false);
    }
}







