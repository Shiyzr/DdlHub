#include "SlideMenu.h"

SlideMenu::SlideMenu(QWidget *parent) : QWidget(parent), isVisible(false), menuWidth(200), menuHeight(540), startHeight(60)
{
    if (parent) {
        menuWidth = static_cast<int>(parent->height()/ 6.0);
        menuHeight = parent->height();
    }
    setGeometry(-menuWidth, startHeight, menuWidth, menuHeight);
    setStyleSheet("background-color: rgba(0,0,0,75);");
    setAttribute(Qt::WA_StyledBackground, true); // 确保样式表背景生效

    animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(100);
    animation->setEasingCurve(QEasingCurve::Linear);

    layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignTop);
    setLayout(layout);
}

// SlideMenu.cpp 中添加
void SlideMenu::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    if (parentWidget()) {
        menuWidth = parentWidget()->width() / 6;  // 宽度为父窗口的1/6
        menuHeight = parentWidget()->height();   // 高度等于父窗口高度
        startHeight = parentWidget()->height() / 10;

        // 根据当前可见状态更新位置和大小
        if (isVisible) {
            showMenu();
        } else {
            hideMenu();
        }
    }
    QWidget::resizeEvent(event);
}
void SlideMenu::addWidget(QWidget *widget)
{
    layout->addWidget(widget);
}
void SlideMenu::toggle()
{
    if (isVisible)
        hideMenu();
    else
        showMenu();
}
void SlideMenu::showMenu()
{
    animation->stop();
    animation->setStartValue(geometry());
    animation->setEndValue(QRect(0, startHeight, menuWidth, menuHeight)); // 使用当前计算的高度
    animation->start();
    isVisible = true;
}

void SlideMenu::hideMenu()
{
    animation->stop();
    animation->setStartValue(geometry());
    animation->setEndValue(QRect(-menuWidth, startHeight, menuWidth, menuHeight));
    animation->start();
    isVisible = false;
}
