#include "dleditclass.h"

DLEditClass::DLEditClass(QWidget *parent) : QWidget(parent) {
    setWindowTitle("配电线路编辑");
    setGeometry(0, 0, 400, 588); // 设置宽度为400

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Header
    QHBoxLayout *headerLayout = new QHBoxLayout();


    headerLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum));

    headerLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum));
    mainLayout->addLayout(headerLayout);

    // Line separator
    mainLayout->addWidget(new QFrame());

    // Main content layout
    QHBoxLayout *contentLayout = new QHBoxLayout();
    QGraphicsView *graphicsView = new QGraphicsView();
    contentLayout->addWidget(graphicsView);

    QVBoxLayout *infoLayout = new QVBoxLayout();

    // ID label
    QHBoxLayout *idLayout = new QHBoxLayout();
    QLabel *idLabel = new QLabel("未选择");
    idLabel->setStyleSheet("font: 75 14pt 'Arial';");
    idLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum));
    idLayout->addWidget(idLabel);
    idLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum));
    infoLayout->addLayout(idLayout);

    // Input fields
    QGridLayout *gridLayout = new QGridLayout();
    rotationEdit = new QLineEdit();
    positionXEdit = new QLineEdit();
    positionYEdit = new QLineEdit();

    gridLayout->addWidget(new QLabel("旋转角："), 0, 0);
    gridLayout->addWidget(rotationEdit, 0, 1);
    gridLayout->addWidget(new QLabel("位置X："), 1, 0);
    gridLayout->addWidget(positionXEdit, 1, 1);
    gridLayout->addWidget(new QLabel("位置Y："), 2, 0);
    gridLayout->addWidget(positionYEdit, 2, 1);
    infoLayout->addLayout(gridLayout);

    // Line separator
    infoLayout->addWidget(new QFrame());

    // Type label
    infoLayout->addWidget(new QLabel("类型："));

    // Add the info layout to the main layout
    contentLayout->addLayout(infoLayout);
    mainLayout->addLayout(contentLayout);

    setLayout(mainLayout);
}
