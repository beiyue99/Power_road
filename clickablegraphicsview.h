#ifndef CLICKABLEGRAPHICSVIEW_H
#define CLICKABLEGRAPHICSVIEW_H
#include <QGraphicsView>
#include <QMouseEvent>


//视图类，左侧三个元件所在窗口即是一个视图,重写点击事件，点击后绘制元件到电路图


class ClickableGraphicsView : public QGraphicsView {
    Q_OBJECT

public:
    explicit ClickableGraphicsView(QWidget* parent = nullptr) : QGraphicsView(parent) {}

signals:
    void clicked();  // 当鼠标点击时发出信号

protected:
    // 重写鼠标点击事件
    void mousePressEvent(QMouseEvent* event) ;
};

#endif // CLICKABLEGRAPHICSVIEW_H
