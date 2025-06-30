#include "CustomMenuButton.h"
#include <QPainter>
#include <QStyleOptionButton>

CustomMenuButton::CustomMenuButton(QWidget *parent)
    : QPushButton(parent)
{
    setFixedSize(32, 32); // 你可以根据需求调整
    setCursor(Qt::PointingHandCursor);
    setToolTip("打开全局导航菜单");
    setStyleSheet("background: transparent; border: none;");
}

void CustomMenuButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen(QColor(120, 120, 120)); // 灰色
    pen.setWidthF(1.5);
    painter.setPen(pen);

    // 中心对齐画三条线
    int lineWidth = width() * 0.5;
    int x = (width() - lineWidth) / 2;
    int yStart = height() / 2 - 6;

    pen.setWidthF(0.5);
    painter.setPen(pen);
    painter.drawRoundedRect(rect(), 10, 10);

    int lineHeight = 2;
    int lineSpacing = 6;

    for (int i = 0; i < 3; ++i) {
        QRect lineRect(x, yStart + i * lineSpacing, lineWidth, lineHeight);
        painter.drawLine(lineRect.topLeft(), lineRect.topRight());
    }
}
