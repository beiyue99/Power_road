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
    view->setMinimumWidth(800); // 设置最小宽度为400像素

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
    mainLayout->addWidget(view, 5);       // 中间电路编辑区，设置权重为 3
    mainLayout->addWidget(rightWidget, 0); // 右侧元件详细信息布局




    setupConnections();

}



void CircuitEditor::setupConnections() {


        connect(lampView, &ClickableGraphicsView::clicked, this, [this]() {
            // 创建新的灯泡并添加到场景
            CircuitComponent* newLamp = createLamp(lampCounter++);
            scene->addComponent(newLamp);

        });

        connect(switchView, &ClickableGraphicsView::clicked, this, [this]() {
            // 创建新的开关并添加到场景
            CircuitComponent* newSwitch = createSwitch(switchCounter++);
            scene->addComponent(newSwitch);

        });

        connect(powerView, &ClickableGraphicsView::clicked, this, [this]() {
            // 创建新的电源并添加到场景
            CircuitComponent* newPower = createPower(powerCounter++);
            scene->addComponent(newPower);

        });

        //点击电路中元件，更新右侧相关信息
        connect(scene, &CircuitScene::itemClicked, this, [this](CircuitComponent* component) {
                updateComponentDetails(component);
            });

     //更新旋转角
    connect(rotationEdit, &QLineEdit::editingFinished, this, &CircuitEditor::onRotationChanged);

    // 连接开关状态控制按钮
    connect(disconnectButton, &QPushButton::clicked, this, &CircuitEditor::handleDisconnect);
    connect(connectButton, &QPushButton::clicked, this, &CircuitEditor::handleConnect);

}




//combobox当前选择的东西被修改后，调这个函数更新保存开关和对应combobox状态的容器
void CircuitEditor::comboBoxChanged() {
    CircuitComponent* selectedComponent = scene->getSelectedComponent();
    if (!selectedComponent) {
        return;  // 如果没有选中的组件，提前返回
    }

    // 检查选中的组件是否为开关
    if (selectedComponent->getType() == "开关") {
        switchComboBoxStates[selectedComponent->getName()] = QStringList({
            comboBox1->currentText(),
            comboBox2->currentText(),
            comboBox3->currentText(),
            comboBox4->currentText(),
            comboBox5->currentText(),
            comboBox6->currentText()
        });
    }

    // 清空并重新填充 comboBox，但先断开信号连接，防止触发 comboBoxChanged
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

    // 更新开关1
    qDebug() << "更新开关1";
    comboBox1->clear();
    comboBox1->addItem("");
    for(int i = 0; i < m_ComboboxItems.at(0).size(); i++){
        if(m_ComboboxItems.at(0).at(i) == comboBox2->currentText() || m_ComboboxItems.at(0).at(i) == comboBox3->currentText() || m_ComboboxItems.at(0).at(i) == ""){
        }else{
            comboBox1->addItem(m_ComboboxItems.at(0).at(i));
        }
    }
    qDebug() << switchComboBoxStates[selectedComponent->getName()].at(0);
    comboBox1->setCurrentText(switchComboBoxStates[selectedComponent->getName()].at(0));
    // 更新开关2
    qDebug() << "更新开关2";
    comboBox2->clear();
    comboBox2->addItem("");
    for(int i = 0; i < m_ComboboxItems.at(1).size(); i++){
        if(m_ComboboxItems.at(1).at(i) == comboBox1->currentText() || m_ComboboxItems.at(1).at(i) == comboBox3->currentText() || m_ComboboxItems.at(1).at(i) == ""){
        }else{
            comboBox2->addItem(m_ComboboxItems.at(1).at(i));
        }
    }
    comboBox2->setCurrentText(switchComboBoxStates[selectedComponent->getName()].at(1));
    // 更新开关3
    comboBox3->clear();
    comboBox3->addItem("");
    for(int i = 0; i < m_ComboboxItems.at(2).size(); i++){
        if(m_ComboboxItems.at(2).at(i) == comboBox1->currentText() || m_ComboboxItems.at(2).at(i) == comboBox2->currentText() || m_ComboboxItems.at(2).at(i) == ""){
        }else{
            comboBox3->addItem(m_ComboboxItems.at(2).at(i));
        }
    }
    comboBox3->setCurrentText(switchComboBoxStates[selectedComponent->getName()].at(2));



    // 更新开关4
    qDebug() << "更新开关4";
    comboBox4->clear();
    comboBox4->addItem("");
    for(int i = 0; i < m_ComboboxItems.at(3).size(); i++){
        if(m_ComboboxItems.at(3).at(i) == comboBox5->currentText() || m_ComboboxItems.at(3).at(i) == comboBox6->currentText() || m_ComboboxItems.at(0).at(i) == ""){
        }else{
            comboBox4->addItem(m_ComboboxItems.at(3).at(i));
        }
    }
    qDebug() << switchComboBoxStates[selectedComponent->getName()].at(3);
    comboBox4->setCurrentText(switchComboBoxStates[selectedComponent->getName()].at(3));
    // 更新开关5
    qDebug() << "更新开关5";
    comboBox5->clear();
    comboBox5->addItem("");
    for(int i = 0; i < m_ComboboxItems.at(4).size(); i++){
        if(m_ComboboxItems.at(4).at(i) == comboBox4->currentText() || m_ComboboxItems.at(4).at(i) == comboBox6->currentText() || m_ComboboxItems.at(4).at(i) == ""){
        }else{
            comboBox5->addItem(m_ComboboxItems.at(4).at(i));
        }
    }
    comboBox5->setCurrentText(switchComboBoxStates[selectedComponent->getName()].at(4));
    // 更新开关6
    comboBox6->clear();
    comboBox6->addItem("");
    for(int i = 0; i < m_ComboboxItems.at(5).size(); i++){
        if(m_ComboboxItems.at(5).at(i) == comboBox4->currentText() || m_ComboboxItems.at(5).at(i) == comboBox5->currentText() || m_ComboboxItems.at(5).at(i) == ""){
        }else{
            comboBox6->addItem(m_ComboboxItems.at(5).at(i));
        }
    }
    comboBox6->setCurrentText(switchComboBoxStates[selectedComponent->getName()].at(5));

    // 重新连接信号
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





void CircuitEditor::updateComboBoxes(CircuitComponent* component) {
    if (!component) {
        qDebug() << "传入的组件无效！";
        return;
    }

    if (component->getType() != "开关") {
        return;
    }

    // 获取当前的 comboBox 状态
    QStringList savedValues = switchComboBoxStates.value(component->getName(),
                                                         QStringList({"", "", "", "", "", ""}));

    // 清空并重新填充 comboBox，但先断开信号连接，防止触发 comboBoxChanged
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

    QStringList validOptions1, validOptions2, validOptions3, validOptions4, validOptions5, validOptions6;

    validOptions1 << "";
    validOptions2 << "";
    validOptions3 << "";
    validOptions4 << "";
    validOptions5 << "";
    validOptions6 << "";

    for (CircuitComponent* comp : scene->getAllComponents()) {
        if (comp->getName() != component->getName()) {
            if (comp->getType() == "开关") {
                validOptions1 << comp->getName() + "-1端" << comp->getName() + "-2端";
                validOptions2 << comp->getName() + "-1端" << comp->getName() + "-2端";
                validOptions3 << comp->getName() + "-1端" << comp->getName() + "-2端";
                validOptions4 << comp->getName() + "-1端" << comp->getName() + "-2端";
                validOptions5 << comp->getName() + "-1端" << comp->getName() + "-2端";
                validOptions6 << comp->getName() + "-1端" << comp->getName() + "-2端";
            } else {
                validOptions1 << comp->getName();
                validOptions2 << comp->getName();
                validOptions3 << comp->getName();
                validOptions4 << comp->getName();
                validOptions5 << comp->getName();
                validOptions6 << comp->getName();
            }
        }
    }

    m_ComboboxItems = {validOptions1, validOptions2, validOptions3, validOptions4, validOptions5, validOptions6};
    comboBox1->clear();
    comboBox1->addItems(validOptions1);
    comboBox2->clear();
    comboBox2->addItems(validOptions2);
    comboBox3->clear();
    comboBox3->addItems(validOptions3);
    comboBox4->clear();
    comboBox4->addItems(validOptions4);
    comboBox5->clear();
    comboBox5->addItems(validOptions5);
    comboBox6->clear();
    comboBox6->addItems(validOptions6);


    // 重新设置之前选中的选项
    comboBox1->setCurrentText(savedValues[0]);
    comboBox2->setCurrentText(savedValues[1]);
    comboBox3->setCurrentText(savedValues[2]);
    comboBox4->setCurrentText(savedValues[3]);
    comboBox5->setCurrentText(savedValues[4]);
    comboBox6->setCurrentText(savedValues[5]);



    // 更新开关1
    qDebug() << "更新开关1";
    comboBox1->clear();
    comboBox1->addItem("");
    for(int i = 0; i < m_ComboboxItems.at(0).size(); i++){
        if(m_ComboboxItems.at(0).at(i) == comboBox2->currentText() || m_ComboboxItems.at(0).at(i) == comboBox3->currentText() || m_ComboboxItems.at(0).at(i) == ""){
        }else{
            comboBox1->addItem(m_ComboboxItems.at(0).at(i));
        }
    }
    comboBox1->setCurrentText(savedValues[0]);
    // 更新开关2
    qDebug() << "更新开关2";
    comboBox2->clear();
    comboBox2->addItem("");
    for(int i = 0; i < m_ComboboxItems.at(1).size(); i++){
        if(m_ComboboxItems.at(1).at(i) == comboBox1->currentText() || m_ComboboxItems.at(1).at(i) == comboBox3->currentText() || m_ComboboxItems.at(1).at(i) == ""){
        }else{
            comboBox2->addItem(m_ComboboxItems.at(1).at(i));
        }
    }
    comboBox2->setCurrentText(savedValues[1]);
    // 更新开关3
    comboBox3->clear();
    comboBox3->addItem("");
    for(int i = 0; i < m_ComboboxItems.at(2).size(); i++){
        if(m_ComboboxItems.at(2).at(i) == comboBox1->currentText() || m_ComboboxItems.at(2).at(i) == comboBox2->currentText() || m_ComboboxItems.at(2).at(i) == ""){
        }else{
            comboBox3->addItem(m_ComboboxItems.at(2).at(i));
        }
    }
    comboBox3->setCurrentText(savedValues[2]);



    // 更新开关4
    qDebug() << "更新开关4";
    comboBox4->clear();
    comboBox4->addItem("");
    for(int i = 0; i < m_ComboboxItems.at(3).size(); i++){
        if(m_ComboboxItems.at(3).at(i) == comboBox5->currentText() || m_ComboboxItems.at(3).at(i) == comboBox6->currentText() || m_ComboboxItems.at(0).at(i) == ""){
        }else{
            comboBox4->addItem(m_ComboboxItems.at(3).at(i));
        }
    }
    comboBox4->setCurrentText(savedValues[3]);
    // 更新开关5
    qDebug() << "更新开关5";
    comboBox5->clear();
    comboBox5->addItem("");
    for(int i = 0; i < m_ComboboxItems.at(4).size(); i++){
        if(m_ComboboxItems.at(4).at(i) == comboBox4->currentText() || m_ComboboxItems.at(4).at(i) == comboBox6->currentText() || m_ComboboxItems.at(4).at(i) == ""){
        }else{
            comboBox5->addItem(m_ComboboxItems.at(4).at(i));
        }
    }
    comboBox5->setCurrentText(savedValues[4]);
    // 更新开关6
    comboBox6->clear();
    comboBox6->addItem("");
    for(int i = 0; i < m_ComboboxItems.at(5).size(); i++){
        if(m_ComboboxItems.at(5).at(i) == comboBox4->currentText() || m_ComboboxItems.at(5).at(i) == comboBox5->currentText() || m_ComboboxItems.at(5).at(i) == ""){
        }else{
            comboBox6->addItem(m_ComboboxItems.at(5).at(i));
        }
    }
    comboBox6->setCurrentText(savedValues[5]);

    // 重新连接信号
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
    }
}

void CircuitEditor::handleConnect() {
    CircuitComponent* selectedComponent = scene->getSelectedComponent();
    if (selectedComponent && selectedComponent->getType() == "开关") {
        selectedComponent->setClosed(true);  // 设置为闭合状态
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


void CircuitEditor::clearComboBoxes() {
    comboBox1->clear();
    comboBox2->clear();
    comboBox3->clear();
    comboBox4->clear();
    comboBox5->clear();
    comboBox6->clear();
}






