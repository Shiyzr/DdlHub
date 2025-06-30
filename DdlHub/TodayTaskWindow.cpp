// #include "TodayTaskWindow.h"
// #include <QVBoxLayout>
// #include <QScrollArea>
// #include <QLabel>
// #include <QPushButton>
// #include <QDateTime>
// #include <QMessageBox>
// #include <QDebug>

// TodayTaskWindow::TodayTaskWindow(User &user, QWidget *parent)
//     : QWidget(parent), m_user(user)
// {
//     setupUI();
//     loadTasks();
// }

// void TodayTaskWindow::setupUI()
// {
//     // 主布局
//     m_mainLayout = new QVBoxLayout(this);

//     // 标题
//     QLabel *titleLabel = new QLabel("今日任务清单", this);
//     titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; margin-bottom: 10px;");
//     m_mainLayout->addWidget(titleLabel);

//     // 滚动区域
//     m_scrollArea = new QScrollArea(this);
//     m_scrollArea->setWidgetResizable(true);

//     // 任务容器
//     m_tasksContainer = new QWidget();
//     m_tasksLayout = new QVBoxLayout(m_tasksContainer);
//     m_tasksLayout->setAlignment(Qt::AlignTop);
//     m_tasksLayout->setSpacing(10);

//     m_scrollArea->setWidget(m_tasksContainer);
//     m_mainLayout->addWidget(m_scrollArea);
// }

// void TodayTaskWindow::loadTasks()
// {
//     // 清空现有任务
//     QLayoutItem *child;
//     while ((child = m_tasksLayout->takeAt(0))) {
//         delete child->widget();
//         delete child;
//     }

//     // 获取当前星期几 (0-6, 0=周日)
//     int currentDay = QDate::currentDate().dayOfWeek() - 1;
//     if (currentDay < 0) currentDay = 6; // 调整周日为6

//     // 今日23:59
//     QDateTime todayEnd = QDateTime(QDate::currentDate(), QTime(23, 59, 59));

//     // 分类任务
//     QList<Task> todayTasks;   // 今日任务
//     QList<Task> longTermTasks; // 长期任务

//     for (const Task &task : m_user.tasks) {
//         if (task.isCompleted || task.failed) continue;

//         // 检查是否是今日任务 (A类)
//         if (!task.cycle.empty()) {
//             if (task.cycle.contains(currentDay)) {
//                 todayTasks.append(task);
//                 continue;
//             }
//         }

//         // 检查是否是长期任务 (B类且剩余时间<7天)
//         int daysRemaining = QDateTime::currentDateTime().daysTo(task.deadline);
//         if (daysRemaining < 7) {
//             longTermTasks.append(task);
//         }
//     }

//     // 排序函数
//     auto sortTasks = [&](Task &a, Task &b) {
//         // 先按剩余时间排序
//         int aRemaining, bRemaining;

//         if (&a >= todayTasks.constBegin() && &a < todayTasks.constEnd()) {
//             aRemaining = QDateTime::currentDateTime().secsTo(todayEnd);
//         } else {
//             aRemaining = QDateTime::currentDateTime().secsTo(a.deadline);
//         }

//         if (&b >= todayTasks.constBegin() && &b < todayTasks.constEnd()) {
//             bRemaining = QDateTime::currentDateTime().secsTo(todayEnd);
//         } else {
//             bRemaining = QDateTime::currentDateTime().secsTo(b.deadline);
//         }

//         if (aRemaining != bRemaining) {
//             return aRemaining < bRemaining;
//         }

//         // 剩余时间相同，按类型排序 (Assignment < Paper < Exam)
//         return a.type < b.type;
//     };

//     // 排序任务
//     std::sort(todayTasks.begin(), todayTasks.end(), sortTasks);
//     std::sort(longTermTasks.begin(), longTermTasks.end(), sortTasks);

//     // 添加今日任务标题
//     if (!todayTasks.isEmpty()) {
//         QLabel *todayLabel = new QLabel("今日任务", m_tasksContainer);
//         todayLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #2c3e50; margin-top: 10px;");
//         m_tasksLayout->addWidget(todayLabel);

//         // 添加今日任务
//         for (const Task &task : todayTasks) {
//             addTaskWidget(task, true);
//         }
//     }

//     // 添加长期任务标题
//     if (!longTermTasks.isEmpty()) {
//         QLabel *longTermLabel = new QLabel("即将到期 (7天内)", m_tasksContainer);
//         longTermLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #2c3e50; margin-top: 10px;");
//         m_tasksLayout->addWidget(longTermLabel);

//         // 添加长期任务
//         for (const Task &task : longTermTasks) {
//             addTaskWidget(task, false);
//         }
//     }

//     // 如果没有任务
//     if (todayTasks.isEmpty() && longTermTasks.isEmpty()) {
//         QLabel *noTaskLabel = new QLabel("今天没有任务，好好休息吧！", m_tasksContainer);
//         noTaskLabel->setStyleSheet("font-size: 14px; color: #7f8c8d;");
//         noTaskLabel->setAlignment(Qt::AlignCenter);
//         m_tasksLayout->addWidget(noTaskLabel);
//     }
// }

// void TodayTaskWindow::addTaskWidget(const Task &task, bool isTodayTask)
// {
//     QWidget *taskWidget = new QWidget(m_tasksContainer);
//     taskWidget->setStyleSheet("QWidget { background-color: #f5f5f5; border-radius: 5px; padding: 10px; }");

//     QHBoxLayout *taskLayout = new QHBoxLayout(taskWidget);
//     taskLayout->setContentsMargins(10, 10, 10, 10);

//     // 完成按钮
//     QPushButton *completeButton = new QPushButton("完成", taskWidget);
//     completeButton->setStyleSheet(
//         "QPushButton { background-color: #2ecc71; color: white; border: none; padding: 5px 10px; border-radius: 3px; }"
//         "QPushButton:hover { background-color: #27ae60; }"
//     );

//     // 存储任务索引以便完成时使用
//     int taskIndex = m_user.tasks.indexOf(task);
//     completeButton->setProperty("taskIndex", taskIndex);

//     connect(completeButton, &QPushButton::clicked, this, &TodayTaskWindow::onTaskCompleted);

//     // 任务信息
//     QVBoxLayout *infoLayout = new QVBoxLayout();

//     // 任务名称
//     QLabel *nameLabel = new QLabel(task.title, taskWidget);
//     nameLabel->setStyleSheet("font-weight: bold; font-size: 14px;");

//     // 任务类型
//     QString typeStr;
//     switch (task.type) {
//         case Assignment: typeStr = "作业"; break;
//         case Paper: typeStr = "论文"; break;
//         case Exam: typeStr = "考试"; break;
//     }
//     QLabel *typeLabel = new QLabel("类型: " + typeStr, taskWidget);
//     typeLabel->setStyleSheet("color: #7f8c8d; font-size: 12px;");

//     // 剩余时间
//     QString timeRemainingStr;
//     if (isTodayTask) {
//         int secs = QDateTime::currentDateTime().secsTo(QDateTime(QDate::currentDate(), QTime(23, 59, 59)));
//         int hours = secs / 3600;
//         int mins = (secs % 3600) / 60;
//         timeRemainingStr = QString("剩余: %1小时%2分钟").arg(hours).arg(mins);
//     } else {
//         int days = QDateTime::currentDateTime().daysTo(task.deadline);
//         if (days == 0) {
//             int secs = QDateTime::currentDateTime().secsTo(task.deadline);
//             int hours = secs / 3600;
//             int mins = (secs % 3600) / 60;
//             timeRemainingStr = QString("剩余: %1小时%2分钟").arg(hours).arg(mins);
//         } else {
//             timeRemainingStr = QString("剩余: %1天").arg(days);
//         }
//     }
//     QLabel *timeLabel = new QLabel(timeRemainingStr, taskWidget);
//     timeLabel->setStyleSheet("color: #e74c3c; font-size: 12px;");

//     infoLayout->addWidget(nameLabel);
//     infoLayout->addWidget(typeLabel);
//     infoLayout->addWidget(timeLabel);

//     taskLayout->addWidget(completeButton);
//     taskLayout->addLayout(infoLayout, 1);

//     m_tasksLayout->addWidget(taskWidget);
// }

// void TodayTaskWindow::onTaskCompleted(int taskIndex)
// {
//     if (taskIndex >= 0 && taskIndex < m_user.tasks.size()) {
//         Task &task = m_user.tasks[taskIndex];

//         QMessageBox::StandardButton reply;
//         reply = QMessageBox::question(this, "确认完成",
//                                      QString("确认完成任务「%1」吗？").arg(task.title),
//                                      QMessageBox::Yes | QMessageBox::No);

//         if (reply == QMessageBox::Yes) {
//             task.isCompleted = true;
//             m_user.taskcompletetime++;
//             loadTasks(); // 刷新任务列表

//             // 保存用户数据
//             DataManager manager;
//             manager.saveUser(m_user);
//         }
//     }
// }




#include "TodayTaskWindow.h"
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include <QDateTime>
#include <QMessageBox>
#include <QDebug>

TodayTaskWindow::TodayTaskWindow(User &user, QWidget *parent)
    : QWidget(parent), m_user(user)
{
    setupUI();
    loadTasks();
    completedTasks=false;
}

void TodayTaskWindow::setupUI()
{
    // 全局样式设置（深色背景 + 白色粗体文字）
    this->setStyleSheet(R"(
        QWidget {
            background-color: #1e1e1e;
            color: white;
            font-weight: bold;
        }
        QLabel {
            color: white;
        }
        QScrollArea {
            background-color: #1e1e1e;
        }
        QPushButton {
            color: white;
            font-weight: bold;
        }
    )");

    // 主布局
    m_mainLayout = new QVBoxLayout(this);

    // 标题
    QLabel *titleLabel = new QLabel("今日任务清单", this);
    titleLabel->setStyleSheet("font-size: 18px; margin-bottom: 10px;");
    m_mainLayout->addWidget(titleLabel);

    // 滚动区域
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);

    // 任务容器
    m_tasksContainer = new QWidget();
    m_tasksLayout = new QVBoxLayout(m_tasksContainer);
    m_tasksLayout->setAlignment(Qt::AlignTop);
    m_tasksLayout->setSpacing(10);

    m_scrollArea->setWidget(m_tasksContainer);
    m_mainLayout->addWidget(m_scrollArea);
}

void TodayTaskWindow::loadTasks()
{
    QLayoutItem *child;
    while ((child = m_tasksLayout->takeAt(0))) {
        delete child->widget();
        delete child;
    }

    int currentDay = QDate::currentDate().dayOfWeek() - 1;
    if (currentDay < 0) currentDay = 6;

    QDateTime todayEnd = QDateTime(QDate::currentDate(), QTime(23, 59, 59));



    for (const Task &task : m_user.tasks) {
        if (task.isCompleted || task.failed) continue;

        if (!task.cycle.empty()) {
            if (task.cycle.contains(currentDay)) {
                todayTasks.append(task);
                continue;
            }
        }

        int daysRemaining = QDateTime::currentDateTime().daysTo(task.deadline);
        if (daysRemaining < 7) {
            longTermTasks.append(task);
        }
    }

    auto sortTasks = [&](Task &a, Task &b) {
        int aRemaining, bRemaining;

        if (&a >= todayTasks.constBegin() && &a < todayTasks.constEnd()) {
            aRemaining = QDateTime::currentDateTime().secsTo(todayEnd);
        } else {
            aRemaining = QDateTime::currentDateTime().secsTo(a.deadline);
        }

        if (&b >= todayTasks.constBegin() && &b < todayTasks.constEnd()) {
            bRemaining = QDateTime::currentDateTime().secsTo(todayEnd);
        } else {
            bRemaining = QDateTime::currentDateTime().secsTo(b.deadline);
        }

        if (aRemaining != bRemaining) {
            return aRemaining < bRemaining;
        }

        return a.type < b.type;
    };

    std::sort(todayTasks.begin(), todayTasks.end(), sortTasks);
    std::sort(longTermTasks.begin(), longTermTasks.end(), sortTasks);

    if (!todayTasks.isEmpty()) {
        QLabel *todayLabel = new QLabel("今日任务", m_tasksContainer);
        todayLabel->setStyleSheet("font-size: 16px; margin-top: 10px;");
        m_tasksLayout->addWidget(todayLabel);

        for (const Task &task : todayTasks) {
            addTaskWidget(task, true);
        }
    }

    if (!longTermTasks.isEmpty()) {
        QLabel *longTermLabel = new QLabel("即将到期 (7天内)", m_tasksContainer);
        longTermLabel->setStyleSheet("font-size: 16px; margin-top: 10px;");
        m_tasksLayout->addWidget(longTermLabel);

        for (const Task &task : longTermTasks) {
            addTaskWidget(task, false);
        }
    }

    if (todayTasks.isEmpty() && longTermTasks.isEmpty()) {
        QLabel *noTaskLabel = new QLabel("今天没有任务，好好休息吧！", m_tasksContainer);
        noTaskLabel->setStyleSheet("font-size: 14px; color: #bdc3c7;");
        noTaskLabel->setAlignment(Qt::AlignCenter);
        m_tasksLayout->addWidget(noTaskLabel);
    }
}

// void TodayTaskWindow::addTaskWidget(const Task &task, bool isTodayTask)
// {
//     QWidget *taskWidget = new QWidget(m_tasksContainer);
//     taskWidget->setStyleSheet("QWidget { background-color: #2c2c2c; border-radius: 5px; padding: 10px; }");

//     QHBoxLayout *taskLayout = new QHBoxLayout(taskWidget);
//     taskLayout->setContentsMargins(10, 10, 10, 10);

//     QPushButton *completeButton = new QPushButton("完成", taskWidget);
//     completeButton->setStyleSheet(
//         "QPushButton { background-color: #2ecc71; color: white; border: none; padding: 5px 10px; border-radius: 3px; font-weight: bold; }"
//         "QPushButton:hover { background-color: #58d68d; }"
//         );

//     int taskIndex = m_user.tasks.indexOf(task);
//     completeButton->setProperty("taskIndex", taskIndex);

//     connect(completeButton, &QPushButton::clicked, this, &TodayTaskWindow::onTaskCompleted);

//     QVBoxLayout *infoLayout = new QVBoxLayout();

//     QLabel *nameLabel = new QLabel(task.title, taskWidget);
//     nameLabel->setStyleSheet("font-size: 14px;");

//     QString typeStr;
//     switch (task.type) {
//     case Assignment: typeStr = "作业"; break;
//     case Paper: typeStr = "论文"; break;
//     case Exam: typeStr = "考试"; break;
//     }
//     QLabel *typeLabel = new QLabel("类型: " + typeStr, taskWidget);
//     typeLabel->setStyleSheet("font-size: 12px; color: #95a5a6;");

//     QString timeRemainingStr;
//     if (isTodayTask) {
//         int secs = QDateTime::currentDateTime().secsTo(QDateTime(QDate::currentDate(), QTime(23, 59, 59)));
//         int hours = secs / 3600;
//         int mins = (secs % 3600) / 60;
//         timeRemainingStr = QString("剩余: %1小时%2分钟").arg(hours).arg(mins);
//     } else {
//         int days = QDateTime::currentDateTime().daysTo(task.deadline);
//         if (days == 0) {
//             int secs = QDateTime::currentDateTime().secsTo(task.deadline);
//             int hours = secs / 3600;
//             int mins = (secs % 3600) / 60;
//             timeRemainingStr = QString("剩余: %1小时%2分钟").arg(hours).arg(mins);
//         } else {
//             timeRemainingStr = QString("剩余: %1天").arg(days);
//         }
//     }
//     QLabel *timeLabel = new QLabel(timeRemainingStr, taskWidget);
//     timeLabel->setStyleSheet("font-size: 12px; color: #e74c3c;");

//     infoLayout->addWidget(nameLabel);
//     infoLayout->addWidget(typeLabel);
//     infoLayout->addWidget(timeLabel);

//     taskLayout->addWidget(completeButton);
//     taskLayout->addLayout(infoLayout, 1);

//     m_tasksLayout->addWidget(taskWidget);
// }


/*oid TodayTaskWindow::addTaskWidget(const Task &task, bool isTodayTask)
{
    QWidget *taskWidget = new QWidget(m_tasksContainer);
    taskWidget->setStyleSheet("QWidget { background-color: #2c2c2c; border-radius: 4px; padding: 5px; }");

    QHBoxLayout *taskLayout = new QHBoxLayout(taskWidget);
    taskLayout->setContentsMargins(8, 4, 8, 4);  // 更紧凑
    taskLayout->setSpacing(12);

    // 完成按钮
    QPushButton *completeButton = new QPushButton("完成", taskWidget);
    completeButton->setStyleSheet(
        "QPushButton { background-color: #2ecc71; color: white; border: none; padding: 4px 8px; border-radius: 3px; font-weight: bold; font-size: 12px; }"
        "QPushButton:hover { background-color: #58d68d; }"
        );
    int taskIndex = m_user.tasks.indexOf(task);
    completeButton->setProperty("taskIndex", taskIndex);
    connect(completeButton, &QPushButton::clicked, this, &TodayTaskWindow::onTaskCompleted);

    // 名称标签
    QLabel *nameLabel = new QLabel(task.title, taskWidget);
    nameLabel->setStyleSheet("font-size: 13px; font-weight: bold; color: white;");
    nameLabel->setFixedWidth(150);  // 固定宽度避免挤压其它元素

    // 类型标签
    QString typeStr;
    switch (task.type) {
    case Assignment: typeStr = "作业"; break;
    case Paper: typeStr = "论文"; break;
    case Exam: typeStr = "考试"; break;
    }
    QLabel *typeLabel = new QLabel("类型: " + typeStr, taskWidget);
    typeLabel->setStyleSheet("font-size: 12px; color: #95a5a6;");
    typeLabel->setFixedWidth(80);

    // 时间剩余标签
    QString timeRemainingStr;
    if (isTodayTask) {
        int secs = QDateTime::currentDateTime().secsTo(QDateTime(QDate::currentDate(), QTime(23, 59, 59)));
        int hours = secs / 3600;
        int mins = (secs % 3600) / 60;
        timeRemainingStr = QString("剩余: %1小时%2分钟").arg(hours).arg(mins);
    } else {
        int days = QDateTime::currentDateTime().daysTo(task.deadline);
        if (days == 0) {
            int secs = QDateTime::currentDateTime().secsTo(task.deadline);
            int hours = secs / 3600;
            int mins = (secs % 3600) / 60;
            timeRemainingStr = QString("剩余: %1小时%2分钟").arg(hours).arg(mins);
        } else {
            timeRemainingStr = QString("剩余: %1天").arg(days);
        }
    }

    QLabel *timeLabel = new QLabel(timeRemainingStr, taskWidget);
    timeLabel->setStyleSheet("font-size: 12px; color: #e74c3c;");
    //timeLabel->setFixedWidth(110);
    timeLabel->setMinimumWidth(110);
    timeLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    // 加入布局
    taskLayout->addWidget(completeButton);
    taskLayout->addWidget(nameLabel);
    taskLayout->addWidget(typeLabel);
    taskLayout->addWidget(timeLabel);
    taskLayout->addStretch();  // 占位符拉伸末尾

    m_tasksLayout->addWidget(taskWidget);
}



void TodayTaskWindow::onTaskCompleted(int taskIndex)
{
    if (taskIndex >= 0 && taskIndex < m_user.tasks.size()) {
        Task &task = m_user.tasks[taskIndex];

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "确认完成",
                                      QString("确认完成任务「%1」吗？").arg(task.title),
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            task.isCompleted = true;
            m_user.taskcompletetime++;
            loadTasks();

            DataManager manager;
            manager.saveUser(m_user);
        }
    }
}*/
void TodayTaskWindow::addTaskWidget(const Task &task, bool isTodayTask)
{
    QWidget *taskWidget = new QWidget(m_tasksContainer);
    taskWidget->setStyleSheet("QWidget { background-color: #2c2c2c; border-radius: 4px; padding: 5px; }");

    QHBoxLayout *taskLayout = new QHBoxLayout(taskWidget);
    taskLayout->setContentsMargins(8, 4, 8, 4);
    taskLayout->setSpacing(12);

    QPushButton *completeButton = new QPushButton("完成", taskWidget);
    completeButton->setStyleSheet(
        "QPushButton { background-color: #2ecc71; color: white; border: none; padding: 4px 8px; border-radius: 3px; font-weight: bold; font-size: 12px; }"
        "QPushButton:hover { background-color: #58d68d; }"
        );

    // Store task title instead of ID
    completeButton->setProperty("taskTitle", task.title);
    connect(completeButton, &QPushButton::clicked, this, [this, task]() {
        onTaskCompleted(task.title);
    });

    QLabel *nameLabel = new QLabel(task.title, taskWidget);
    nameLabel->setStyleSheet("font-size: 13px; font-weight: bold; color: white;");
    nameLabel->setFixedWidth(150);

    QString typeStr;
    switch (task.type) {
    case Assignment: typeStr = "任务"; break;
    case Paper: typeStr = "论文"; break;
    case Exam: typeStr = "考试"; break;
    }
    QLabel *typeLabel = new QLabel("类型: " + typeStr, taskWidget);
    typeLabel->setStyleSheet("font-size: 12px; color: #95a5a6;");
    typeLabel->setFixedWidth(80);

    QString timeRemainingStr;
    if (isTodayTask) {
        int secs = QDateTime::currentDateTime().secsTo(QDateTime(QDate::currentDate(), QTime(23, 59, 59)));
        int hours = secs / 3600;
        int mins = (secs % 3600) / 60;
        timeRemainingStr = QString("剩余: %1小时%2分钟").arg(hours).arg(mins);
    } else {
        int days = QDateTime::currentDateTime().daysTo(task.deadline);
        if (days == 0) {
            int secs = QDateTime::currentDateTime().secsTo(task.deadline);
            int hours = secs / 3600;
            int mins = (secs % 3600) / 60;
            timeRemainingStr = QString("剩余: %1小时%2分钟").arg(hours).arg(mins);
        } else {
            timeRemainingStr = QString("剩余: %1天").arg(days);
        }
    }

    QLabel *timeLabel = new QLabel(timeRemainingStr, taskWidget);
    timeLabel->setStyleSheet("font-size: 12px; color: #e74c3c;");
    timeLabel->setMinimumWidth(110);
    timeLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    taskLayout->addWidget(completeButton);
    taskLayout->addWidget(nameLabel);
    taskLayout->addWidget(typeLabel);
    taskLayout->addWidget(timeLabel);
    taskLayout->addStretch();

    m_tasksLayout->addWidget(taskWidget);
}

void TodayTaskWindow::onTaskCompleted(const QString& taskTitle)
{
    // Find task by title
    auto it = std::find_if(m_user.tasks.begin(), m_user.tasks.end(),
                           [taskTitle](const Task& t) { return t.title == taskTitle; });

    if (it != m_user.tasks.end()) {
        Task& task = *it;

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "确认完成",
                                      QString("确认完成任务「%1」吗？").arg(task.title),
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            if (task.cycle.empty()) {
                // Non-recurring task - delete it
                m_user.completetask(it->title);
                m_user.tasks.erase(it);
                completedTasks=true;
            } else {
                // Recurring task - just mark as completed
                m_user.completetask(it->title);
            }

            m_user.taskcompletetime++;
            loadTasks();

            DataManager manager;
            manager.saveUser(m_user);
        }
    }
}
