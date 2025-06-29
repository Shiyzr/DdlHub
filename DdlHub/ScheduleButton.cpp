// ScheduleButton.cpp
#include "ScheduleButton.h"
#include <QPainter>
#include <QStyleOptionButton>

ScheduleButton::ScheduleButton(QWidget *parent)
    : QPushButton(parent) {
    setFixedSize(40, 40); // 可根据需要调整大小
}

void ScheduleButton::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QStyleOptionButton option;
    option.initFrom(this);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 画按钮背景
    style()->drawControl(QStyle::CE_PushButton, &option, &painter, this);

    // 九宫格参数
    const int squareSize = 6;       // 小正方形边长
    const int spacing = 4;          // 间距
    const int offsetX = (width() - (3 * squareSize + 2 * spacing)) / 2;
    const int offsetY = (height() - (3 * squareSize + 2 * spacing)) / 2;

    painter.setBrush(QBrush(Qt::black));
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(255,255,255)));

    // 绘制 3x3 正方形
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            int x = offsetX + col * (squareSize + spacing);
            int y = offsetY + row * (squareSize + spacing);
            painter.drawRect(x, y, squareSize, squareSize);
        }
    }
}
