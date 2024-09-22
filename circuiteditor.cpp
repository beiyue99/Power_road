#include "circuiteditor.h"
#include "circuitcomponent.h"

#include "circuitscene.h"  // 添加这一行



CircuitEditor::CircuitEditor(QWidget *parent) : QWidget(parent), powerCounter(1), switchCounter(1), lampCounter('A')
{
    // 创建主布局：水平布局
    QHBoxLayout* mainLayout = new QHBoxLayout(this);

    // 左侧布局：电路元件按钮，图形在上，名字在下
    QVBoxLayout* leftLayout = new QVBoxLayout();

    // 灯泡部分
    QVBoxLayout* lampLayout = new QVBoxLayout();
    QLabel* lampLabel = new QLabel("灯泡");
    ClickableGraphicsView* lampView = new ClickableGraphicsView();
    QGraphicsScene* lampScene = new QGraphicsScene();
    lampScene->addItem(createLampWithoutLabel());  // 使用不带编号的灯泡图形
    lampView->setScene(lampScene);
    lampView->setFrameShape(QFrame::NoFrame);   // 去掉灯泡视图的边框
    lampLayout->addWidget(lampView);
    lampLayout->addWidget(lampLabel);

    // 开关部分
    QVBoxLayout* switchLayout = new QVBoxLayout();
    QLabel* switchLabel = new QLabel("开关");
    ClickableGraphicsView* switchView = new ClickableGraphicsView();
    QGraphicsScene* switchScene = new QGraphicsScene();
    switchScene->addItem(createSwitchWithoutLabel());  // 使用不带编号的开关图形
    switchView->setScene(switchScene);
    switchView->setFrameShape(QFrame::NoFrame); // 去掉开关视图的边框
    switchLayout->addWidget(switchView);
    switchLayout->addWidget(switchLabel);

    // 电源部分
    QVBoxLayout* powerLayout = new QVBoxLayout();
    QLabel* powerLabel = new QLabel("电源");
    ClickableGraphicsView* powerView = new ClickableGraphicsView();
    QGraphicsScene* powerScene = new QGraphicsScene();
    powerScene->addItem(createPowerWithoutLabel());  // 使用不带编号的电源图形
    powerView->setScene(powerScene);
    powerView->setFrameShape(QFrame::NoFrame);  // 去掉电源视图的边框
    powerLayout->addWidget(powerView);
    powerLayout->addWidget(powerLabel);

    // 将灯泡、开关、电源布局添加到左侧布局
    leftLayout->addLayout(lampLayout);
    leftLayout->addLayout(switchLayout);
    leftLayout->addLayout(powerLayout);
    leftLayout->addStretch(); // 空间填充


    // 中间布局：电路编辑区域（自定义的 CircuitScene）
    QGraphicsView* view = new QGraphicsView(this);
    CircuitScene* scene = new CircuitScene(this);  // 使用自定义的 CircuitScene
    view->setScene(scene);




    // 将布局加入主布局中
    mainLayout->addLayout(leftLayout,1); // 左侧电路元件按钮布局
    mainLayout->addWidget(view,4);       // 中间电路编辑区

    // 点击事件：添加带编号的元件到中间电路区域
    connect(lampView, &ClickableGraphicsView::clicked, [this, scene]() {
        scene->addItem(createLamp(lampCounter++));  // 添加灯泡并递增字母编号
    });

    connect(switchView, &ClickableGraphicsView::clicked, [this, scene]() {
        scene->addItem(createSwitch(switchCounter++, false));  // 默认水平
    });

    connect(powerView, &ClickableGraphicsView::clicked, [this, scene]() {
        scene->addItem(createPower(powerCounter++));  // 添加电源并递增编号
    });


}









