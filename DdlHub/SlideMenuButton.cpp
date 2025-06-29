#include "SlideMenuButton.h"

#include <QPainter>
#include <QMouseEvent>

SlideMenuButton::SlideMenuButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent), m_hovered(false)
{
    setMinimumSize(80, 40);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    setCursor(Qt::PointingHandCursor);
}

SlideMenuButton::SlideMenuButton(QWidget *parent)
    : QPushButton(parent), m_hovered(false)
{
    setText("默认按钮");
    setMinimumSize(80, 30);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    setCursor(Qt::PointingHandCursor);
}

void SlideMenuButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QColor bgColor = m_hovered ? QColor(0,0,0,100) : QColor(0,0,0,0);
    painter.setBrush(bgColor);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), 10, 10);

    // 文字
    painter.setPen(Qt::white);
    painter.setFont(QFont("微软雅黑", 12, QFont::Bold));
    painter.drawText(rect(), Qt::AlignCenter, text());  // 使用父类的 text()
    this->setStyleSheet("text-align: left; padding-left: 10px;");

}

void SlideMenuButton::enterEvent(QEnterEvent *)
{
    m_hovered = true;
    update();
}

void SlideMenuButton::leaveEvent(QEvent *)
{
    m_hovered = false;
    update();
}
