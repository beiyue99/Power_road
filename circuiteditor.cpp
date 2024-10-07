#include "circuiteditor.h"
#include "circuitcomponent.h"
#include "circuitscene.h"  // 添加这一行
#include <QFrame>

CircuitEditor::CircuitEditor(QWidget *parent)
    : QWidget(parent), powerCounter(1), switchCounter(1), lampCounter('A') {
    // 创建主布局：水平布局
    QHBoxLayout* mainLayout = new QHBoxLayout(this);

    // 左侧布局：电路元件按钮，图形在上，名字在下
    QWidget* leftWidget = new QWidget(this);
    leftWidget->setFixedSize(110, 500); // 设置固定大小
    QVBoxLayout* leftLayout = new QVBoxLayout(leftWidget);

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
    view->setMinimumWidth(800); // 设置最小宽度为800像素

    // 右侧布局：元件详细信息
    QWidget* rightWidget = new QWidget(this);
    rightWidget->setFixedSize(150, 500); // 设置固定大小
    QVBoxLayout* rightLayout = new QVBoxLayout(rightWidget);

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
    mainLayout->addWidget(leftWidget, 0); // 左侧电路元件按钮布局
    mainLayout->addWidget(view, 5);       // 中间电路编辑区，设置权重为 5
    mainLayout->addWidget(rightWidget, 0); // 右侧元件详细信息布局

    setupConnections();
}

void CircuitEditor::setupConnections() {


    connect(lampView, &ClickableGraphicsView::clicked, this, [this]() {
           // 创建新的灯泡并添加到场景
           CircuitComponent* newLamp = createLamp(lampCounter++);
           scene->addComponent(newLamp);
           // 清除当前选择的项
           scene->clearSelection();
           // 选中新添加的灯泡
           newLamp->setSelected(true);
           // 更新右侧信息
           updateComponentDetails(newLamp);
       });

       connect(switchView, &ClickableGraphicsView::clicked, this, [this]() {
           // 创建新的开关并添加到场景
           CircuitComponent* newSwitch = createSwitch(switchCounter++);
           scene->addComponent(newSwitch);
           // 清除当前选择的项
           scene->clearSelection();
           // 选中新添加的开关
           newSwitch->setSelected(true);
           // 更新右侧信息
           updateComponentDetails(newSwitch);
       });

       connect(powerView, &ClickableGraphicsView::clicked, this, [this]() {
           // 创建新的电源并添加到场景
           CircuitComponent* newPower = createPower(powerCounter++);
           scene->addComponent(newPower);
           // 清除当前选择的项
           scene->clearSelection();
           // 选中新添加的电源
           newPower->setSelected(true);
           // 更新右侧信息
           updateComponentDetails(newPower);
       });
        //点击电路中元件，更新右侧相关信息
        connect(scene, &CircuitScene::itemClicked, this, [this](CircuitComponent* component) {
                updateComponentDetails(component);
            });
        connect(scene, &CircuitScene::componentDragged, this, [this](CircuitComponent* component) {

                updateComponentDetails(component);
                 component->updateWires(); // 更新连线位置
            });
        // 点击非电路元件，更新右侧显示为默认状态
          connect(scene, &CircuitScene::noItemClicked, this, [this]() {
              clearComponentDetails();
          });
          connect(scene, &CircuitScene::componentMoved, this, [this](CircuitComponent* component) {
              if (scene->getSelectedComponent() == component) {
                  updateComponentDetails(component);
              }
          });

     //更新旋转角
    connect(rotationEdit, &QLineEdit::editingFinished, this, &CircuitEditor::onRotationChanged);
    // 连接开关状态控制按钮
    connect(disconnectButton, &QPushButton::clicked, this, &CircuitEditor::handleDisconnect);
    connect(connectButton, &QPushButton::clicked, this, &CircuitEditor::handleConnect);

}

void CircuitEditor::clearComponentDetails() {
    // 清空右侧显示
    nameLabel->setText("元件名称");
    rotationEdit->clear();
    posXEdit->clear();
    posYEdit->clear();
    typeLabel->setText("类型：");

    // 隐藏按钮和 comboBox
    disconnectButton->setVisible(false);
    connectButton->setVisible(false);
    label1->setVisible(false);
    label2->setVisible(false);
    comboBox1->setVisible(false);
    comboBox2->setVisible(false);
    comboBox3->setVisible(false);
    comboBox4->setVisible(false);
    comboBox5->setVisible(false);
    comboBox6->setVisible(false);
}

void CircuitEditor::comboBoxChanged() {
    CircuitComponent* selectedComponent = scene->getSelectedComponent();
    if (!selectedComponent || selectedComponent->getType() != "开关") {
        return;
    }

    // 保存当前开关的组合框状态
    switchComboBoxStates[selectedComponent->getName()] = QStringList{
        comboBox1->currentText(),
        comboBox2->currentText(),
        comboBox3->currentText(),
        comboBox4->currentText(),
        comboBox5->currentText(),
        comboBox6->currentText()
    };

    // 更新连线
    updateWiresForComponent(selectedComponent);

    // 更新组合框选项，避免重复选择
    updateComboBoxes(selectedComponent);

    // 调用回路检测
    scene->updatePowerStatus();
}


void CircuitEditor::updateWiresForComponent(CircuitComponent* component) {
    // 首先，清除当前组件的所有连线
    component->removeAllWires();

    // 映射每个 comboBox 到对应的端点（1端或2端）
    QMap<QComboBox*, QString> comboBoxEndMap = {
        {comboBox1, "1端"},
        {comboBox2, "1端"},
        {comboBox3, "1端"},
        {comboBox4, "2端"},
        {comboBox5, "2端"},
        {comboBox6, "2端"}
    };

    // 遍历每个 comboBox，创建连线
    for (QComboBox* comboBox : comboBoxEndMap.keys()) {
        QString targetText = comboBox->currentText();
        if (!targetText.isEmpty()) {
            // 解析目标组件名称和端点
            QString targetComponentName;
            QString targetEndType;
            if (targetText.contains("-")) {
                QStringList parts = targetText.split("-");
                targetComponentName = parts[0];
                targetEndType = parts[1];
            } else {
                targetComponentName = targetText;
                targetEndType = ""; // 对于灯泡和电源，无需指定端点
            }

            // 查找目标组件
            CircuitComponent* targetComponent = nullptr;
            for (CircuitComponent* comp : scene->getAllComponents()) {
                if (comp->getName() == targetComponentName) {
                    targetComponent = comp;
                    break;
                }
            }

            if (targetComponent) {
                // 创建连线，指定正确的起点和终点端点类型
                QString componentEnd = comboBoxEndMap[comboBox]; // 当前组件的端点（1端或2端）
                CircuitWire* wire = new CircuitWire(component, targetComponent, componentEnd, targetEndType);
                scene->addItem(wire);
                component->addWire(componentEnd, wire);

            }
        }
    }

    // 更新连线位置
    component->updateWires();
    // CircuitScene::updateCircuitStatus 会被自动调用 via CircuitScene::updateWires
}

void CircuitEditor::updateComboBoxes(CircuitComponent* component) {
    if (!component || component->getType() != "开关") {
        return;
    }

    // 获取当前开关保存的组合框状态
    QStringList savedValues = switchComboBoxStates.value(component->getName(),
                                                         QStringList() << "" << "" << "" << "" << "" << "");

    // 断开信号连接，防止触发 comboBoxChanged
    disconnectCMB();

    // 获取所有元件的名称列表
    QStringList componentNames;
    for (CircuitComponent* comp : scene->getAllComponents()) {
        if (comp->getName() != component->getName()) {
            if (comp->getType() == "开关") {
                componentNames << comp->getName() + "-1端" << comp->getName() + "-2端";
            } else {
                componentNames << comp->getName();
            }
        }
    }

    // 更新每个组合框的选项并设置默认值
    QComboBox* comboBoxes[6] = {comboBox1, comboBox2, comboBox3, comboBox4, comboBox5, comboBox6};

    for (int i = 0; i < 6; ++i) {
        comboBoxes[i]->clear();
        comboBoxes[i]->addItem("");  // 添加空选项

        QStringList options = componentNames;

        // 排除同一端已选中的项（保留当前组合框的值）
        QStringList excludes;
        if (i < 3) {
            // 1端
            for (int j = 0; j < 3; ++j) {
                if (j != i && !savedValues[j].isEmpty()) {
                    excludes << savedValues[j];
                }
            }
        } else {
            // 2端
            for (int j = 3; j < 6; ++j) {
                if (j != i && !savedValues[j].isEmpty()) {
                    excludes << savedValues[j];
                }
            }
        }

        for (const QString& option : options) {
            if (!excludes.contains(option)) {
                comboBoxes[i]->addItem(option);
            }
        }

        // 设置当前值
        if (!savedValues[i].isEmpty() && comboBoxes[i]->findText(savedValues[i]) != -1) {
            comboBoxes[i]->setCurrentText(savedValues[i]);
        } else {
            comboBoxes[i]->setCurrentIndex(0);
        }
    }

    // 重新连接信号
    connectCMB();
}

void CircuitEditor::updateComponentDetails(CircuitComponent* component) {

    if (!component) {
           qDebug() << "传入的组件无效！";
           return;  // 如果组件无效，提前返回
       }

    // 更新元件名称
    QString newName = component->getName();
    if (nameLabel->text() != newName) {
        nameLabel->setText(newName);
        nameLabel->setAlignment(Qt::AlignCenter);  // 确保居中对齐
    }

    // 更新旋转角度
    double rotation = component->rotation();
    if (rotationEdit->text() != QString::number(rotation)) {
        rotationEdit->setText(QString::number(rotation));
    }

    // 更新位置X
    double posX = component->pos().x();
    if (posXEdit->text() != QString::number(posX)) {
        posXEdit->setText(QString::number(posX));
    }

    // 更新位置Y
    double posY = component->pos().y();
    if (posYEdit->text() != QString::number(posY)) {
        posYEdit->setText(QString::number(posY));
    }

    // 更新元件类型
    QString newType = component->getType();
    if (typeLabel->text() != "类型：" + newType) {
        typeLabel->setText("类型：" + newType);
        typeLabel->setAlignment(Qt::AlignCenter);  // 确保居中对齐
    }

    // 根据类型显示或隐藏按钮和 comboBox
    bool isSwitch = (newType == "开关");
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

    // 更新连接元件的 comboBox
    if (isSwitch) {
        updateComboBoxes(component);  // 更新连接元件的选项
    }
}

void CircuitEditor::handleDisconnect() {
    CircuitComponent* selectedComponent = scene->getSelectedComponent();
    if (selectedComponent && selectedComponent->getType() == "开关") {
        selectedComponent->setClosed(false);  // 设置为断开状态
        scene->updatePowerStatus();    // 调用回路检测
    }
}

void CircuitEditor::handleConnect() {
    CircuitComponent* selectedComponent = scene->getSelectedComponent();
    if (selectedComponent && selectedComponent->getType() == "开关") {
        selectedComponent->setClosed(true);   // 设置为闭合状态
        scene->updatePowerStatus();    // 调用回路检测
    }
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

void CircuitEditor::connectCMB()
{
    connect(comboBox1, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CircuitEditor::comboBoxChanged);
    connect(comboBox2, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CircuitEditor::comboBoxChanged);
    connect(comboBox3, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CircuitEditor::comboBoxChanged);
    connect(comboBox4, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CircuitEditor::comboBoxChanged);
    connect(comboBox5, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CircuitEditor::comboBoxChanged);
    connect(comboBox6, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CircuitEditor::comboBoxChanged);
}

void CircuitEditor::disconnectCMB()
{
    disconnect(comboBox1, QOverload<int>::of(&QComboBox::currentIndexChanged),
               this, &CircuitEditor::comboBoxChanged);
    disconnect(comboBox2, QOverload<int>::of(&QComboBox::currentIndexChanged),
               this, &CircuitEditor::comboBoxChanged);
    disconnect(comboBox3, QOverload<int>::of(&QComboBox::currentIndexChanged),
               this, &CircuitEditor::comboBoxChanged);
    disconnect(comboBox4, QOverload<int>::of(&QComboBox::currentIndexChanged),
               this, &CircuitEditor::comboBoxChanged);
    disconnect(comboBox5, QOverload<int>::of(&QComboBox::currentIndexChanged),
               this, &CircuitEditor::comboBoxChanged);
    disconnect(comboBox6, QOverload<int>::of(&QComboBox::currentIndexChanged),
               this, &CircuitEditor::comboBoxChanged);
}
