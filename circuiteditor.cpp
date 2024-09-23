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
    lampLabel->setAlignment(Qt::AlignCenter);
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
    switchLabel->setAlignment(Qt::AlignCenter);
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
    powerLabel->setAlignment(Qt::AlignCenter);
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
    scene = new CircuitScene(this);
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




    // 创建布局
    QVBoxLayout* comboxLayout = new QVBoxLayout();

    // 1端布局
    QVBoxLayout* end1Layout = new QVBoxLayout();
    label1 = new QLabel("1端:");
    label1->setStyleSheet("font-weight: bold;"); // 加粗显示
    comboBox1 = new QComboBox();
    comboBox2 = new QComboBox();
    comboBox3 = new QComboBox();

    // 将1端的控件添加到布局
    end1Layout->addWidget(label1);
    end1Layout->addWidget(comboBox1);
    end1Layout->addWidget(comboBox2);
    end1Layout->addWidget(comboBox3);

    // 2端布局
    QVBoxLayout* end2Layout = new QVBoxLayout();
    label2 = new QLabel("2端:");
    label2->setStyleSheet("font-weight: bold;"); // 加粗显示
    comboBox4 = new QComboBox();
    comboBox5 = new QComboBox();
    comboBox6 = new QComboBox();

    // 将2端的控件添加到布局
    end2Layout->addWidget(label2);
    end2Layout->addWidget(comboBox4);
    end2Layout->addWidget(comboBox5);
    end2Layout->addWidget(comboBox6);

    // 将1端和2端布局添加到主布局
    comboxLayout->addLayout(end1Layout);
    comboxLayout->addLayout(end2Layout);

    // 添加主布局到右侧布局
    rightLayout->addLayout(comboxLayout);

    // 隐藏1端和2端的控件
    label1->setVisible(false);
    label2->setVisible(false);
    comboBox1->setVisible(false);
    comboBox2->setVisible(false);
    comboBox3->setVisible(false);
    comboBox4->setVisible(false);
    comboBox5->setVisible(false);
    comboBox6->setVisible(false);




    // 将布局加入主布局中
    mainLayout->addLayout(leftLayout, 1); // 左侧电路元件按钮布局
    mainLayout->addWidget(view, 4);       // 中间电路编辑区
    mainLayout->addLayout(rightLayout, 1); // 右侧元件详细信息布局

    // 点击事件：添加带编号的元件到中间电路区域
    connect(lampView, &ClickableGraphicsView::clicked, [this]() {
        scene->addItem(createLamp(lampCounter++));
    });

    connect(switchView, &ClickableGraphicsView::clicked, [this]() {
        scene->addItem(createSwitch(switchCounter++));
    });

    connect(powerView, &ClickableGraphicsView::clicked, [this]() {
        scene->addItem(createPower(powerCounter++));
    });

    connect(scene, &CircuitScene::itemClicked, this, &CircuitEditor::updateComponentDetails);

    // 在构造函数中连接旋转角度的信号
    connect(rotationEdit, &QLineEdit::editingFinished, this, &CircuitEditor::onRotationChanged);

}







void CircuitEditor::onRotationChanged() {
    bool ok;
    double angle = rotationEdit->text().toDouble(&ok);
    if (ok) {
        // 获取当前选中的元件
        CircuitComponent* selectedComponent = scene->getSelectedComponent();
        if (selectedComponent) {
            selectedComponent->setRotation(angle); // 旋转指定角度
            selectedComponent->update(); // 更新显示
        }
    }
}





void CircuitEditor::updateComponentDetails(CircuitComponent* component) {
    nameLabel->setText(component->getName());
    nameLabel->setAlignment(Qt::AlignCenter);  // 确保居中对齐
    rotationEdit->setText(QString::number(component->rotation()));
    posXEdit->setText(QString::number(component->pos().x()));
    posYEdit->setText(QString::number(component->pos().y()));
    typeLabel->setText("类型：" + component->getType());
    typeLabel->setAlignment(Qt::AlignCenter);  // 确保居中对齐

    // 根据类型显示或隐藏按钮和 comboBox
    bool isSwitch = component->getType() == "开关";

    disconnectButton->setVisible(isSwitch);
    connectButton->setVisible(isSwitch);
    label1->setVisible(isSwitch);
    label2->setVisible(isSwitch);
    comboBox1->setVisible(isSwitch);
    comboBox2->setVisible(isSwitch);
    comboBox3->setVisible(isSwitch);
    comboBox4->setVisible(isSwitch);
    comboBox5->setVisible(isSwitch);
    comboBox6->setVisible(isSwitch);
}










