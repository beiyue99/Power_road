#include "circuiteditor.h"
#include "circuitcomponent.h"
#include "circuitscene.h"  // 添加这一行
#include <QFrame>

CircuitEditor::CircuitEditor(QWidget *parent)
    : QWidget(parent), powerCounter(1), switchCounter(1), lampCounter('A') {
    // 创建主布局，改为垂直布局
    QVBoxLayout* mainVerticalLayout = new QVBoxLayout(this);

    // 创建原有的主水平布局
    QHBoxLayout* mainLayout = new QHBoxLayout();

    // 左侧布局：电路元件按钮，图形在上，名字在下
    leftWidget = new QWidget(this);
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

    deleteButton = new QPushButton("删除元件");

    // 将删除按钮添加到界面的右侧布局
    rightLayout->addWidget(deleteButton);
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

    // 将主水平布局添加到主垂直布局
    mainLayout->addWidget(leftWidget, 0); // 左侧电路元件按钮布局
    mainLayout->addWidget(view, 5);       // 中间电路编辑区，设置权重为 5
    mainLayout->addWidget(rightWidget, 0); // 右侧元件详细信息布局
    mainVerticalLayout->addLayout(mainLayout);

    // 添加水平分隔线
    QFrame* bottomSeparator = new QFrame();
    bottomSeparator->setFrameShape(QFrame::HLine);
    bottomSeparator->setFrameShadow(QFrame::Sunken);
    mainVerticalLayout->addWidget(bottomSeparator);

    // 添加底部的按钮（保存方案和打开方案）
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    saveButton = new QPushButton("保存方案");
    loadButton = new QPushButton("打开方案");

    // 将按钮添加到布局中，并设置居中
    buttonLayout->addStretch();
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(loadButton);
    buttonLayout->addStretch();
    mainVerticalLayout->addLayout(buttonLayout);


    setupConnections();  // 保留其他原有的信号槽连接
}


void CircuitEditor::setupConnections() {


    // 创建新的灯泡
        connect(lampView, &ClickableGraphicsView::clicked, this, [this]() {
            CircuitComponent* newLamp = createLamp(QString(QChar(lampCounter++)));
            scene->addComponent(newLamp);
            scene->clearSelection();
            newLamp->setSelected(true);
            updateComponentDetails(newLamp);
        });

        // 创建新的开关
        connect(switchView, &ClickableGraphicsView::clicked, this, [this]() {
            CircuitComponent* newSwitch = createSwitch(switchCounter++);
            scene->addComponent(newSwitch);
            scene->clearSelection();
            newSwitch->setSelected(true);
            updateComponentDetails(newSwitch);
        });

        // 创建新的电源
        connect(powerView, &ClickableGraphicsView::clicked, this, [this]() {
            CircuitComponent* newPower = createPower(powerCounter++);
            scene->addComponent(newPower);
            scene->clearSelection();
            newPower->setSelected(true);
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
    connect(deleteButton, &QPushButton::clicked, this, &CircuitEditor::handleDeleteComponent);
    connect(scene,&CircuitScene::recheck,this,&CircuitEditor::updateWiresForComponent);
    // 连接 X 和 Y 坐标输入框的信号
       connect(posXEdit, &QLineEdit::editingFinished, this, &CircuitEditor::onPositionChanged);
       connect(posYEdit, &QLineEdit::editingFinished, this, &CircuitEditor::onPositionChanged);
       // 连接按钮功能
       connect(saveButton, &QPushButton::clicked, this, &CircuitEditor::saveScheme);
       connect(loadButton, &QPushButton::clicked, this, &CircuitEditor::loadScheme);


}


void CircuitEditor::saveScheme() {
    qDebug() << "Saving scheme...";

    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/scheme.xml";
    QFile file(desktopPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Couldn't open file for writing at " << desktopPath;
        return;
    }

    QDomDocument doc;
    QDomElement root = doc.createElement("CircuitScheme");
    doc.appendChild(root);

    // 遍历所有组件
    for (CircuitComponent* component : scene->getAllComponents()) {
        QDomElement componentElem = doc.createElement("Component");
        componentElem.setAttribute("name", component->getName());
        componentElem.setAttribute("type", component->getType());
        componentElem.setAttribute("x", component->pos().x());
        componentElem.setAttribute("y", component->pos().y());
        componentElem.setAttribute("rotation", component->rotation());
        componentElem.setAttribute("closed", component->isClosed() ? "1" : "0");

        // 如果是开关，保存其组合框状态
        if (component->getType() == "开关") {
            QStringList comboBoxValues = switchComboBoxStates.value(component->getName(), QStringList());
            for (const QString& value : comboBoxValues) {
                QDomElement comboBoxElem = doc.createElement("ComboBox");
                QDomText textNode = doc.createTextNode(value);
                comboBoxElem.appendChild(textNode);
                componentElem.appendChild(comboBoxElem);
            }
        }

        root.appendChild(componentElem);
    }

    // 使用集合来避免重复保存导线
    QSet<CircuitWire*> savedWires;

    // 遍历所有组件，保存其导线
    for (CircuitComponent* component : scene->getAllComponents()) {
        for (const QString& end : component->getWires().keys()) {
            QList<CircuitWire*>& wireList = component->getWires()[end];
            for (CircuitWire* wire : wireList) {
                if (!savedWires.contains(wire)) {
                    savedWires.insert(wire);

                    CircuitComponent* startComponent = dynamic_cast<CircuitComponent*>(wire->getStartItem());
                    CircuitComponent* endComponent = dynamic_cast<CircuitComponent*>(wire->getEndItem());

                    if (startComponent && endComponent) {
                        QDomElement wireElem = doc.createElement("Wire");
                        wireElem.setAttribute("startComponent", startComponent->getName());
                        wireElem.setAttribute("startEndType", wire->getStartEndType());
                        wireElem.setAttribute("endComponent", endComponent->getName());
                        wireElem.setAttribute("endEndType", wire->getEndEndType());
                        root.appendChild(wireElem);
                        qDebug() << "Saving wire between" << startComponent->getName() << "and" << endComponent->getName();
                    } else {
                        qWarning() << "Failed to save wire due to missing components.";
                    }
                }
            }
        }
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");  // 设置为UTF-8编码
    doc.save(out, 4);        // 保存XML
    file.close();

    qDebug() << "Scheme saved to" << desktopPath;

}



void CircuitEditor::loadScheme() {
    qDebug() << "Loading scheme...";

    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/scheme.xml";
    QFile file(desktopPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Couldn't open file for reading at " << desktopPath;
        return;
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");  // 设置为UTF-8编码

    QDomDocument doc;
    if (!doc.setContent(&file)) {
        qWarning() << "Couldn't parse XML.";
        file.close();
        return;
    }
    file.close();

    QDomElement root = doc.documentElement();
    QDomNodeList components = root.elementsByTagName("Component");

    // 清除当前场景的所有组件
    scene->clear();

    // 清除开关的组合框状态
    switchComboBoxStates.clear();

    QMap<QString, CircuitComponent*> componentMap;

    // 新增：用于记录最大的灯泡标签和开关、电源编号
    QChar maxLampLabel = 'A' - 1;
    int maxSwitchNumber = 0;
    int maxPowerNumber = 0;

    // 加载所有元件
    for (int i = 0; i < components.size(); ++i) {
        QDomElement componentElem = components.at(i).toElement();

        QString name = componentElem.attribute("name");
        QString type = componentElem.attribute("type");
        double x = componentElem.attribute("x").toDouble();
        double y = componentElem.attribute("y").toDouble();
        double rotation = componentElem.attribute("rotation").toDouble();
        bool closed = componentElem.attribute("closed").toInt();

        CircuitComponent* component = nullptr;
        if (type == "灯泡") {
            QString label = name.mid(2); // 从名称中提取标识
            component = createLamp(label);

            // 更新最大灯泡标签
            if (!label.isEmpty()) {
                QChar labelChar = label.at(0);
                if (labelChar > maxLampLabel) {
                    maxLampLabel = labelChar;
                }
            }
        } else if (type == "开关") {
            int switchNumber = name.mid(2).toInt();
            component = createSwitch(switchNumber);

            // 更新最大开关编号
            if (switchNumber > maxSwitchNumber) {
                maxSwitchNumber = switchNumber;
            }
        } else if (type == "电源") {
            int powerNumber = name.mid(2).toInt();
            component = createPower(powerNumber);

            // 更新最大电源编号
            if (powerNumber > maxPowerNumber) {
                maxPowerNumber = powerNumber;
            }
        }

        if (component) {
            component->setPos(x, y);
            component->setRotation(rotation);
            component->setClosed(closed);
            scene->addComponent(component);
            componentMap[name] = component; // 将元件添加到映射中
            qDebug() << "Component" << name << "loaded.";

            // 如果是开关，恢复组合框状态
            if (type == "开关") {
                QStringList comboBoxValues;
                QDomNodeList comboBoxNodes = componentElem.elementsByTagName("ComboBox");
                for (int j = 0; j < comboBoxNodes.size(); ++j) {
                    QDomElement comboBoxElem = comboBoxNodes.at(j).toElement();
                    comboBoxValues.append(comboBoxElem.text());
                }
                switchComboBoxStates[name] = comboBoxValues;
            }
        }
    }

    // 更新计数器，确保新添加的元件名称不重复
    if (maxLampLabel >= 'A') {
        lampCounter = maxLampLabel.unicode() + 1;
    } else {
        lampCounter = 'A';
    }
    switchCounter = maxSwitchNumber + 1;
    powerCounter = maxPowerNumber + 1;

    // 加载所有导线
    QDomNodeList wires = root.elementsByTagName("Wire");
    for (int i = 0; i < wires.size(); ++i) {
        QDomElement wireElem = wires.at(i).toElement();

        QString startComponentName = wireElem.attribute("startComponent");
        QString startEndType = wireElem.attribute("startEndType");
        QString endComponentName = wireElem.attribute("endComponent");
        QString endEndType = wireElem.attribute("endEndType");

        CircuitComponent* startComponent = componentMap.value(startComponentName, nullptr);
        CircuitComponent* endComponent = componentMap.value(endComponentName, nullptr);

        if (startComponent && endComponent) {
            qDebug() << "Creating wire between" << startComponentName << "and" << endComponentName;
            CircuitWire* wire = new CircuitWire(startComponent, endComponent, startEndType, endEndType);
            scene->addItem(wire); // 将导线添加到场景中
            startComponent->addWire(startEndType, wire);
            endComponent->addWire(endEndType, wire);
        } else {
            qWarning() << "Failed to create wire between" << startComponentName << "and" << endComponentName;
        }
    }

    // 恢复开关的组合框状态
    for (CircuitComponent* comp : componentMap.values()) {
        if (comp->getType() == "开关") {
            updateComboBoxes(comp);
            // 不再调用 updateWiresForComponent，以避免移除已有的导线
        }
    }

    scene->updatePowerStatus();
    qDebug() << "Scheme loaded from" << desktopPath;
}





void CircuitEditor::onPositionChanged() {
    // 获取当前选中的元件
    CircuitComponent* selectedComponent = scene->getSelectedComponent();
    if (!selectedComponent) {
        return;
    }

    // 获取用户输入的 X 和 Y 坐标
    bool xOk, yOk;
    double x = posXEdit->text().toDouble(&xOk);
    double y = posYEdit->text().toDouble(&yOk);

    // 如果输入合法，则更新元件的位置
    if (xOk && yOk) {
        selectedComponent->setPos(x, y);  // 更新元件位置
        selectedComponent->updateWires(); // 更新元件的连线
        scene->updatePowerStatus();       // 更新电路状态，检查灯泡亮暗等
    }
}


void CircuitEditor::handleDeleteComponent() {
    // 获取选中的元件
    CircuitComponent* selectedComponent = scene->getSelectedComponent();

    if (!selectedComponent) {
        QMessageBox::warning(this, "删除元件", "没有选中的元件！");
        return;
    }

    // 移除选中元件以及所有的连线
    scene->removeComponentAndWires(selectedComponent);

    // 遍历所有开关并更新它们的 ComboBox
    for (CircuitComponent* comp : scene->getAllComponents()) {
        if (comp->getType() == "开关") {
            updateComboBoxes(comp); // 更新 ComboBox
            updateWiresForComponent(comp); // 更新连线
        }
    }

    // 清除右侧详细信息
    clearComponentDetails();
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
    if (component->getType() != "开关") return;

    // 清除当前组件的所有连线
    component->removeAllWires();

    // **新增部分**：遍历所有开关，移除关于当前组件的旧连接信息
    for (CircuitComponent* comp : scene->getAllComponents()) {
        if (comp->getType() == "开关" && comp != component) {
            QStringList& compValues = switchComboBoxStates[comp->getName()];
            if (compValues.isEmpty()) {
                compValues = QStringList() << "" << "" << "" << "" << "" << "";
            }
            // 移除关于当前组件的连接信息
            for (int i = 0; i < compValues.size(); ++i) {
                if (compValues[i].contains(component->getName())) {
                    compValues[i] = "";
                }
            }
            // 如果被移除连接的开关是当前选中的开关，更新其组合框显示
            if (comp == scene->getSelectedComponent()) {
                updateComboBoxes(comp);
            }
        }
    }

    QMap<QComboBox*, QString> comboBoxEndMap = {
        {comboBox1, "1端"},
        {comboBox2, "1端"},
        {comboBox3, "1端"},
        {comboBox4, "2端"},
        {comboBox5, "2端"},
        {comboBox6, "2端"}
    };

    // 用于记录被连接的开关及其对应的端点
    QMap<CircuitComponent*, QList<QPair<QString, QString>>> connectedSwitches;

    // 创建端点到组合框索引的映射
    QMap<QString, QList<int>> endToIndices = {
        {"1端", {0, 1, 2}},
        {"2端", {3, 4, 5}}
    };

    for (QComboBox* comboBox : comboBoxEndMap.keys()) {
        QString targetText = comboBox->currentText();

        if (!targetText.isEmpty()) {
            QString targetComponentName, targetEndType;
            if (targetText.contains("-")) {
                QStringList parts = targetText.split("-");
                targetComponentName = parts[0];
                targetEndType = parts[1];
            } else {
                targetComponentName = targetText;
                targetEndType = "";  // 如果是灯泡或电源
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
                QString componentEnd = comboBoxEndMap[comboBox];  // 当前组件的端点（"1端" 或 "2端"）
                CircuitWire* wire = new CircuitWire(component, targetComponent, componentEnd, targetEndType);
                scene->addItem(wire);
                component->addWire(componentEnd, wire);

                // 如果目标组件是开关，记录下来以便更新
                if (targetComponent->getType() == "开关") {
                    // 保存当前组件的端点和目标组件的端点
                    connectedSwitches[targetComponent].append(qMakePair(componentEnd, targetEndType));
                }
            }
        }
    }

    // 更新连线位置
    component->updateWires();
    scene->updatePowerStatus();

    // 更新被连接开关的组合框状态
    for (auto it = connectedSwitches.begin(); it != connectedSwitches.end(); ++it) {
        CircuitComponent* connectedSwitch = it.key();
        QList<QPair<QString, QString>> connections = it.value();

        // 更新被连接开关的 switchComboBoxStates
        QStringList& connectedSwitchValues = switchComboBoxStates[connectedSwitch->getName()];
        if (connectedSwitchValues.isEmpty()) {
            connectedSwitchValues = QStringList() << "" << "" << "" << "" << "" << "";
        }

        // 创建端点到组合框索引的映射
        QMap<QString, QList<int>> endToIndices = {
            {"1端", {0, 1, 2}},
            {"2端", {3, 4, 5}}
        };

        for (const QPair<QString, QString>& conn : connections) {
            QString currentComponentEnd = conn.first; // 当前组件的端点
            QString connectedEnd = conn.second;       // 被连接开关的端点

            // 找到一个空的组合框索引，属于被连接开关的 connectedEnd（"1端" 或 "2端"）
            int emptyIndex = -1;
            QList<int> indices = endToIndices[connectedEnd];
            for (int i : indices) {
                if (connectedSwitchValues[i].isEmpty()) {
                    emptyIndex = i;
                    break;
                }
            }

            if (emptyIndex != -1) {
                // 记录连接信息，使用当前组件的名称和端点
                connectedSwitchValues[emptyIndex] = component->getName() + "-" + currentComponentEnd;
            } else {
                // 没有空的组合框，可能需要提示用户
                qDebug() << "No empty comboBox for connected switch" << connectedSwitch->getName() << "on end" << connectedEnd;
            }
        }

        // 如果被连接的开关是当前选中的开关，更新其组合框显示
        if (connectedSwitch == scene->getSelectedComponent()) {
            updateComboBoxes(connectedSwitch);
        }
    }
}








void CircuitEditor::updateComboBoxes(CircuitComponent* component) {
    if (!component || component->getType() != "开关") return;

    QStringList savedValues = switchComboBoxStates.value(component->getName(), QStringList() << "" << "" << "" << "" << "" << "");

    disconnectCMB();  // 断开信号连接，防止不必要的触发

    // 用于记录所有已被其他开关占用的端点
    QSet<QString> occupiedEnds;

    // 遍历所有开关，获取其已连接的端点
    for (CircuitComponent* comp : scene->getAllComponents()) {
        if (comp->getType() == "开关" && comp != component) {
            QStringList otherSwitchValues = switchComboBoxStates.value(comp->getName(), QStringList() << "" << "" << "" << "" << "" << "");
            for (const QString& value : otherSwitchValues) {
                if (!value.isEmpty()) {
                    occupiedEnds.insert(value);
                }
            }
        }
    }

    QComboBox* comboBoxes[6] = {comboBox1, comboBox2, comboBox3, comboBox4, comboBox5, comboBox6};

    for (int i = 0; i < 6; ++i) {
        comboBoxes[i]->clear();
        comboBoxes[i]->addItem("");  // 添加空选项

        QStringList options;

        // 构建选项列表
        for (CircuitComponent* comp : scene->getAllComponents()) {
            if (comp->getName() != component->getName()) {
                if (comp->getType() == "开关") {
                    QString end1 = comp->getName() + "-1端";
                    QString end2 = comp->getName() + "-2端";
                    if (!occupiedEnds.contains(end1) || savedValues.contains(end1)) {
                        options << end1;
                    }
                    if (!occupiedEnds.contains(end2) || savedValues.contains(end2)) {
                        options << end2;
                    }
                } else {
                    if (!occupiedEnds.contains(comp->getName()) || savedValues.contains(comp->getName())) {
                        options << comp->getName();
                    }
                }
            }
        }

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

        if (!savedValues[i].isEmpty() && comboBoxes[i]->findText(savedValues[i]) != -1) {
            comboBoxes[i]->setCurrentText(savedValues[i]);
        } else {
            comboBoxes[i]->setCurrentIndex(0);
        }
    }

    connectCMB();  // 重新连接信号
}






void CircuitEditor::updateComponentDetails(CircuitComponent* component) {

    // 更新元件名称
     nameLabel->setText(component->getName());
     nameLabel->setAlignment(Qt::AlignCenter);

     // 更新类型
     typeLabel->setText("类型：" + component->getType());
     typeLabel->setAlignment(Qt::AlignCenter);

     // 更新旋转角度
     rotationEdit->setText(QString::number(component->rotation()));

     // 更新位置X
     posXEdit->setText(QString::number(component->pos().x()));

     // 更新位置Y
     posYEdit->setText(QString::number(component->pos().y()));

    if (component->getType() == "开关") {
        // 为该开关创建自己的组合框
        for (int i = 0; i < 6; ++i) {
            component->comboBoxes[i] = new QComboBox();
            // 添加到界面布局中
        }

        // 将组合框指针传递给组件
        component->setComboBoxes(component->comboBoxes);

        // 连接组合框的信号槽
        for (int i = 0; i < 6; ++i) {
            connect(component->comboBoxes[i], QOverload<int>::of(&QComboBox::currentIndexChanged),
                    this, &CircuitEditor::comboBoxChanged);
        }
    }

    if (component->getType() == "开关") {
         QStringList savedValues = switchComboBoxStates.value(component->getName(), QStringList() << "" << "" << "" << "" << "" << "");
         disconnectCMB();  // 断开信号连接

         QComboBox* comboBoxes[6] = {comboBox1, comboBox2, comboBox3, comboBox4, comboBox5, comboBox6};

         for (int i = 0; i < 6; ++i) {
             comboBoxes[i]->setCurrentText(savedValues[i]);
         }

         connectCMB();  // 重新连接信号
     }
    // 根据类型显示或隐藏按钮和 comboBox
    bool isSwitch = (component->getType() == "开关");
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
