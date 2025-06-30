#ifndef SLIDEMENU_H
#define SLIDEMENU_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QVBoxLayout>

class SlideMenu : public QWidget
{
    Q_OBJECT
public:
    explicit SlideMenu(QWidget *parent = nullptr);

    void toggle();                  // 切换菜单状态
    void showMenu();                // 显示菜单
    void hideMenu();                // 隐藏菜单
    void addWidget(QWidget *widget); // 添加控件到菜单栏中

    bool isVisible;
    int menuWidth;
    int menuHeight;
    int startHeight;
    QPropertyAnimation *animation;
    QVBoxLayout *layout;

protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // SLIDEMENU_H
