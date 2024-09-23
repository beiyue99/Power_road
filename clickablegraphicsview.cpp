#include "clickablegraphicsview.h"


void ClickableGraphicsView::mousePressEvent(QMouseEvent* event)  {
    if (event->button() == Qt::LeftButton) {
        emit clicked();  // 发出点击信号
    }
}
