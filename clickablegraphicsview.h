#ifndef CLICKABLEGRAPHICSVIEW_H
#define CLICKABLEGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>




//视图类，左侧三个元件所在窗口即是一个视图


class ClickableGraphicsView : public QGraphicsView {
    Q_OBJECT

public:
    explicit ClickableGraphicsView(QWidget* parent = nullptr) : QGraphicsView(parent) {}

signals:
    void clicked();  // 当鼠标点击时发出信号

protected:
    // 重写鼠标点击事件
    void mousePressEvent(QMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            emit clicked();  // 发出点击信号
        }
    }
};

#endif // CLICKABLEGRAPHICSVIEW_H
