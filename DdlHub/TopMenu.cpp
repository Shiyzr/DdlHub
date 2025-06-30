#include "TopMenu.h"
#include "CustomMenuButton.h"
#include "ScheduleButton.h"
//#include <QDebug>

TopMenu::TopMenu(QWidget *parent) : QWidget(parent)
{
    // 设置主布局
    layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0); // 去除边距
    layout->setSpacing(0); // 去除间距

    // 1. 最左边的 customBtn
    customBtn = new CustomMenuButton(this);
    customBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(customBtn, 1); // 添加拉伸因子1

    // 2. 中间添加一个伸缩项，使后面的按钮靠右
    layout->addStretch(5); // 较大的拉伸因子使右侧按钮靠右

    // 3. scheduleBtn (profileButton左侧邻近)
    scheduleBtn = new ScheduleButton(this);
    scheduleBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(scheduleBtn, 1); // 添加拉伸因子1
    // 4. 最右边的 profileButton
    profileButton = new QPushButton(this);
    //profileButton->setIcon(QIcon(":/DdlHub_Logo.png"));
    profileButton->setIcon(QIcon("../../DdlHub_Logo.png"));
    profileButton->setFlat(true);
    profileButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(profileButton, 1); // 添加拉伸因子1
}


void TopMenu::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    // 计算基准大小 - 基于高度或宽度的较小比例
    int baseSize = qMin(width() / 12, height());

    // 设置 customBtn 大小
    customBtn->setFixedSize(baseSize, baseSize);

    // 设置 scheduleBtn 大小
    scheduleBtn->setFixedSize(baseSize, baseSize);

    // 设置 profileButton 大小和图标
    profileButton->setFixedSize(baseSize, baseSize);
    profileButton->setIconSize(QSize(baseSize * 0.8, baseSize * 0.8));

    // 调试输出
    // qDebug() << "TopMenu size:" << size()
    //          << "Base size:" << baseSize
    //          << "CustomBtn:" << customBtn->size()
    //          << "ScheduleBtn:" << scheduleBtn->size()
    //          << "ProfileBtn:" << profileButton->size();
}
